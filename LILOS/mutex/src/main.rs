#![no_std]
#![no_main]

use cortex_m_rt::entry;
use stm32g0xx_hal as hal;
use hal::prelude::*;
use hal::rcc::{Config, Prescaler};
use hal::stm32;
use panic_halt as _; 
use lilos::time::{Millis, PeriodicGate};
use lilos::exec::run_tasks;
use lilos::create_mutex;
#[entry]
fn main() -> ! {
    let mut cp = cortex_m::Peripherals::take().unwrap();
    let dp = stm32::Peripherals::take().unwrap();

    let mut rcc = dp.RCC.freeze(Config::hsi(Prescaler::Div2));
    let gpioa = dp.GPIOA.split(&mut rcc);

    let saida1 = gpioa.pa1.into_push_pull_output(); 
    let saida2 = gpioa.pa2.into_push_pull_output(); 
    let saida3 = gpioa.pa3.into_push_pull_output(); 
    let saida4 = gpioa.pa4.into_push_pull_output(); 
    
    lilos::time::initialize_sys_tick(&mut cp.SYST, 8_000_000);

    create_mutex!(saida_mutex, (saida1, saida2, saida3, saida4));
    let tarefa_um = core::pin::pin!(async {
        let mut gate = PeriodicGate::from(Millis(1800)); 
        loop {
            saida_mutex.lock().await.perform(|(saida1, saida2, saida3, saida4)| {
                saida1.set_high().unwrap();
                saida2.set_low().unwrap();
                saida3.set_low().unwrap(); 
                saida4.set_low().unwrap(); 
            });
            gate.next_time().await; 
        }
    });

    let tarefa_dois = core::pin::pin!(async {
        let mut gate = PeriodicGate::from(Millis(1800)); 
        loop {
            saida_mutex.lock().await.perform(|(saida1, saida2, saida3, saida4)| {
                saida2.set_high().unwrap(); 
                saida1.set_low().unwrap(); 
                saida3.set_low().unwrap(); 
                saida4.set_low().unwrap(); 
            });
            gate.next_time().await; 
        }
    });

    let tarefa_tres = core::pin::pin!(async {
        let mut gate = PeriodicGate::from(Millis(1800)); 
        loop {
            saida_mutex.lock().await.perform(|(saida1, saida2, saida3, saida4)| {
                saida3.set_high().unwrap(); 
                saida1.set_low().unwrap(); 
                saida2.set_low().unwrap(); 
                saida4.set_low().unwrap(); 
            });
            gate.next_time().await; 
        }
    });

    let tarefa_quatro = core::pin::pin!(async {
        let mut gate = PeriodicGate::from(Millis(3900)); 
        loop {
            saida_mutex.lock().await.perform(|(saida1, saida2, saida3, saida4)| {
                saida4.set_high().unwrap(); 
                saida1.set_low().unwrap(); 
                saida2.set_low().unwrap(); 
                saida3.set_low().unwrap(); 
            });
            gate.next_time().await;
        }
    });

    
    run_tasks(
        &mut [tarefa_um, tarefa_dois, tarefa_tres, tarefa_quatro], lilos::exec::ALL_TASKS,);
}