#![no_main]
#![no_std]

use cortex_m_rt::entry;
use stm32g0xx_hal::interrupt;
use stm32g0xx_hal as hal;
use hal::prelude::*;
use hal::rcc::{Config, Prescaler};
use hal::stm32;
use lilos::time::Millis;
use lilos::exec::run_tasks;
use lilos::time::sleep_for;
use panic_halt as _; 
use core::sync::atomic::{AtomicBool, Ordering};
use lilos::exec::Notify; 

const PERIODO_SAIDA: lilos::time::Millis = lilos::time::Millis(500); 
static ESTADO_SAIDA: AtomicBool = AtomicBool::new(false);
static ENTRADA_NOTIFY: Notify = Notify::new();

#[entry]
fn main() -> ! {
    let mut cp = cortex_m::Peripherals::take().unwrap();
    let dp = stm32::Peripherals::take().expect("Falha ao obter periféricos");

    let mut rcc = dp.RCC.freeze(Config::hsi(Prescaler::Div2));
    let gpioa = dp.GPIOA.split(&mut rcc);
    let gpioc = dp.GPIOC.split(&mut rcc);

    
    let mut saida = gpioa.pa1.into_push_pull_output();

    
    let entrada = gpioc.pc13.into_pull_up_input();

    
    lilos::time::initialize_sys_tick(&mut cp.SYST, 8_000_000);

    
    let tarefa_saida = core::pin::pin!(async {
        let mut temporizador = lilos::time::PeriodicGate::from(PERIODO_SAIDA);

        loop {
            if ESTADO_SAIDA.load(Ordering::Relaxed) {
                saida.set_high().unwrap();
            } else {
                saida.set_low().unwrap();
            }

            temporizador.next_time().await;
        }
    });

    
    let tarefa_entrada = core::pin::pin!(async {
        loop {
            ENTRADA_NOTIFY.until(|| entrada.is_low().unwrap()).await;
            let estado_atual = ESTADO_SAIDA.load(Ordering::Relaxed);
            ESTADO_SAIDA.store(!estado_atual, Ordering::Relaxed);            
            while entrada.is_low().unwrap() {
                sleep_for(Millis(10)).await; 
            }
        }
    });
    run_tasks(&mut [tarefa_saida, tarefa_entrada], lilos::exec::ALL_TASKS);
}


#[interrupt]
fn EXTI4_15() {
    ENTRADA_NOTIFY.notify();
}
