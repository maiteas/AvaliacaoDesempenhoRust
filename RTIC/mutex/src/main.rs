#![no_std]
#![no_main]


use panic_halt as _;
use stm32g0xx_hal as hal;
use hal::gpio::{Output, PushPull};
use hal::prelude::*;
use hal::rcc::{Config, Prescaler};
use hal::pac;
use rtic::app;
use hal::gpio::{gpioa::PA1, gpioa::PA2, gpioa::PA3, gpioa::PA4};
use systick_monotonic::{Systick, fugit::Duration};
#[app(device = stm32g0xx_hal::stm32, peripherals = true, dispatchers = [SPI1, EXTI4_15, EXTI0_1, EXTI2_3])]
mod app {
    use super::*;

    #[shared]
    struct Shared {
        saida1: PA1<Output<PushPull>>,
        saida2: PA2<Output<PushPull>>,
        saida3: PA3<Output<PushPull>>,
        saida4: PA4<Output<PushPull>>,
    }

    #[local]
    struct Local {}

    #[monotonic(binds = SysTick, default = true)]
    type MonoTimer = Systick<1000>; 

    #[init]
    fn init(cx: init::Context) -> (Shared, Local, init::Monotonics()) {
        let dp: pac::Peripherals = cx.device;
        
        let mut rcc = dp.RCC.freeze(Config::hsi(Prescaler::Div2)); 
        let gpioa = dp.GPIOA.split(&mut rcc);

        let saida1 = gpioa.pa1.into_push_pull_output();
        let saida2 = gpioa.pa2.into_push_pull_output();
        let saida3 = gpioa.pa3.into_push_pull_output();
        let saida4 = gpioa.pa4.into_push_pull_output();
        
        let systick = Systick::new(cx.core.SYST, 8_000_000);

        Tarefa1::spawn().ok();
        Tarefa2::spawn().ok();
        Tarefa3::spawn().ok();
        Tarefa4::spawn().ok();

        (
            Shared { saida1, saida2, saida3, saida4 },
            Local {},
            init::Monotonics(systick),
        )
    }

    #[task(shared = [saida1, saida2, saida3, saida4], priority = 3)]
    fn Tarefa1(mut cx: Tarefa1::Context) {
        cx.shared.saida1.lock(|saida1| saida1.set_high().unwrap());
        cx.shared.saida2.lock(|saida2| saida2.set_low().unwrap());
        cx.shared.saida3.lock(|saida3| saida3.set_low().unwrap());
        cx.shared.saida4.lock(|saida4| saida4.set_low().unwrap());
        Tarefa1::spawn_after(Duration::<u64, 1, 1000>::millis(1200)).ok();
    }

    #[task(shared = [saida1, saida2, saida3, saida4], priority = 3)]
    fn Tarefa2(mut cx: Tarefa2::Context) {
        cx.shared.saida1.lock(|saida1| saida1.set_low().unwrap());
        cx.shared.saida2.lock(|saida2| saida2.set_high().unwrap());
        cx.shared.saida3.lock(|saida3| saida3.set_low().unwrap());
        cx.shared.saida4.lock(|saida4| saida4.set_low().unwrap());
        Tarefa2::spawn_after(Duration::<u64, 1, 1000>::millis(1800)).ok();
    }

    #[task(shared = [saida1, saida2, saida3, saida4], priority = 3)]
    fn Tarefa3(mut cx: Tarefa3::Context) {
        cx.shared.saida1.lock(|saida1| saida1.set_low().unwrap());
        cx.shared.saida2.lock(|saida2| saida2.set_low().unwrap());
        cx.shared.saida3.lock(|saida3| saida3.set_high().unwrap());
        cx.shared.saida4.lock(|saida4| saida4.set_low().unwrap());
        Tarefa3::spawn_after(Duration::<u64, 1, 1000>::millis(2500)).ok();
    }

    #[task(shared = [saida1, saida2, saida3, saida4], priority = 1)]
    fn Tarefa4(mut cx: Tarefa4::Context) {
        cx.shared.saida1.lock(|saida1| saida1.set_low().unwrap());
        cx.shared.saida2.lock(|saida2| saida2.set_low().unwrap());
        cx.shared.saida3.lock(|saida3| saida3.set_low().unwrap());
        cx.shared.saida4.lock(|saida4| saida4.set_high().unwrap());
        Tarefa4::spawn_after(Duration::<u64, 1, 1000>::millis(3900)).ok();
    }
}