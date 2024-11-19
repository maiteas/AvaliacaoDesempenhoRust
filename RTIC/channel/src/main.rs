#![no_main]
#![no_std]


use stm32g0xx_hal as hal;
use hal::pac;
use hal::prelude::*;
use panic_halt as _;
use hal::rcc::{Config, Prescaler};
use rtic::app;
use rtic_monotonics::systick::prelude::*;

use rtic_sync::{channel::*, make_channel};


systick_monotonic!(Mono, 1000);
#[app(device = stm32g0xx_hal::pac, peripherals = true, dispatchers = [SPI1, USART1, USART2])]
mod app {
    use super::*;

    #[local]
    struct Local {
        saida1: hal::gpio::gpioa::PA1<hal::gpio::Output<hal::gpio::PushPull>>,
        saida2: hal::gpio::gpioa::PA2<hal::gpio::Output<hal::gpio::PushPull>>,
        saida3: hal::gpio::gpioa::PA3<hal::gpio::Output<hal::gpio::PushPull>>,
    }

    #[shared]
    struct Shared {}

    const CAPACITY: usize = 5;

    #[init]
    fn init(cx: init::Context) -> (Shared, Local) {
        let (s1, r1) = make_channel!(u32, CAPACITY);
        let (s2, r2) = make_channel!(u32, CAPACITY);

        tarefa_um::spawn(s1).unwrap();
        tarefa_dois::spawn(r1, s2).unwrap();
        tarefa_tres::spawn(r2).unwrap();

        let dp: pac::Peripherals = cx.device;

        let mut rcc = dp.RCC.freeze(Config::hsi(Prescaler::Div2));

        let gpioa = dp.GPIOA.split(&mut rcc);

        Mono::start(cx.core.SYST, 8_000_000);

        let saida1 = gpioa.pa1.into_push_pull_output();
        let saida2 = gpioa.pa2.into_push_pull_output();
        let saida3 = gpioa.pa3.into_push_pull_output();

        (Shared {}, Local { saida1, saida2, saida3 })
    }

    #[task(local = [saida1], priority = 3)]
    async fn tarefa_um(cx: tarefa_um::Context, mut sender: Sender<'static, u32, CAPACITY>) {
        let mut value = 0;

        loop {
            value = if value == 1 { 0 } else { 1 };

            if value == 1 {
                cx.local.saida1.set_high().unwrap();
            } else {
                cx.local.saida1.set_low().unwrap();
            }
            sender.send(value).await.unwrap();

            Mono::delay(900.millis()).await;
        }
    }

    #[task(local = [saida2], priority = 2)]
    async fn tarefa_dois(
        cx: tarefa_dois::Context,
        mut receiver: Receiver<'static, u32, CAPACITY>,
        mut sender: Sender<'static, u32, CAPACITY>,
    ) {
        loop {
            let value = receiver.recv().await.unwrap();
            if value == 1 {
                cx.local.saida2.set_high().unwrap();
            } else {
                cx.local.saida2.set_low().unwrap();
            }
            sender.send(value).await.unwrap();
            Mono::delay(1000.millis()).await;
        }
}

    #[task(local = [saida3], priority = 1)]
    async fn tarefa_tres(cx: tarefa_tres::Context, mut receiver: Receiver<'static, u32, CAPACITY>) {
        loop {
            let value = receiver.recv().await.unwrap();
            if value == 1 {
                cx.local.saida3.set_high().unwrap();
            } else {
                cx.local.saida3.set_low().unwrap();
            }
            Mono::delay(1300.millis()).await;
        }
    }
}