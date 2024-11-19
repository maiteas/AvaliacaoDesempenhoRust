#![no_std]
#![no_main]

use cortex_m_rt::entry;
use stm32g0xx_hal as hal;
use hal::prelude::*;
use hal::stm32;
use lilos::time::{Millis, PeriodicGate};
use lilos::exec::run_tasks;
use lilos::spsc::Queue;
use core::mem::MaybeUninit;
use hal::rcc::{Config, Prescaler};
use panic_halt as _;

const PERIOD: Millis = Millis(900);
const PERIOD2: Millis = Millis(1000);
const PERIOD3: Millis = Millis(1300);

#[entry]
fn main() -> ! {
    let mut cp = cortex_m::Peripherals::take().unwrap();
    let dp = stm32::Peripherals::take().unwrap();

    let mut rcc = dp.RCC.freeze(Config::hsi(Prescaler::Div2));
    let gpioa = dp.GPIOA.split(&mut rcc);

    let mut saida1 = gpioa.pa1.into_push_pull_output();
    let mut saida2 = gpioa.pa2.into_push_pull_output();
    let mut saida3 = gpioa.pa3.into_push_pull_output();

    lilos::time::initialize_sys_tick(&mut cp.SYST, 8_000_000);

    
    let mut storage1_2: [MaybeUninit<u8>; 2] = [MaybeUninit::uninit(); 2];
    let mut storage2_3: [MaybeUninit<u8>; 2] = [MaybeUninit::uninit(); 2];

    let mut fila1 = Queue::new(&mut storage1_2);
    let mut fila2 = Queue::new(&mut storage2_3);

    let (mut pusher1_2, mut popper2) = fila1.split();
    let (mut pusher2_3, mut popper3) = fila2.split();

    
    let tarefa_um = core::pin::pin!(async {
        let mut gate = PeriodicGate::from(PERIOD);
        let mut value = 0;

        loop {
            value = if value == 1 { 0 } else { 1 };

            if value == 1 {
                saida1.set_high().unwrap();
            } else {
                saida1.set_low().unwrap();
            }

            if pusher1_2.can_push() {
                let _ = pusher1_2.try_push(value);
            }

            gate.next_time().await;
        }
    });

    
    let tarefa_dois = core::pin::pin!(async {
        let mut gate = PeriodicGate::from(PERIOD2);
        loop {
            if let Some(value) = popper2.try_pop() {
                if value == 1 {
                    saida2.set_high().unwrap();
                } else {
                    saida2.set_low().unwrap();
                }

                if pusher2_3.can_push() {
                    let _ = pusher2_3.try_push(value);
                }
            }
            gate.next_time().await;
        }
    });

    
    let tarefa_tres = core::pin::pin!(async {
        let mut gate = PeriodicGate::from(PERIOD3);
        loop {
            if let Some(value) = popper3.try_pop() {
                if value == 1 {
                    saida3.set_high().unwrap();
                } else {
                    saida3.set_low().unwrap();
                }
            }

            gate.next_time().await;
        }
    });

    
    run_tasks(&mut [tarefa_um, tarefa_dois, tarefa_tres], lilos::exec::ALL_TASKS);
}