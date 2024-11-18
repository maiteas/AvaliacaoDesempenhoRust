//funcionou com interrupcao
//adaptado de https://dev.to/theembeddedrustacean/embedded-rust-embassy-gpio-button-controlled-blinking-3ee6
#![no_std]
#![no_main]
extern crate core;

use core::sync::atomic::{AtomicBool, Ordering};
use embassy_executor::Spawner;
use embassy_stm32::exti::ExtiInput;
use embassy_stm32::gpio::{Output, Input, Pull, Level, Speed};
use embassy_stm32::peripherals::{PA2, PA1, PA4}; 
use embassy_time::{Duration, Timer};
use panic_halt as _;

static SAIDA_STATE: AtomicBool = AtomicBool::new(false);


#[embassy_executor::task]
async fn tarefa_um(mut saida2: Output<'static, PA1>) {
    loop {
        saida2.toggle();
        Timer::after(Duration::from_millis(500)).await;
    }
}

#[embassy_executor::task]
async fn tarefa_saida(mut saida: Output<'static, PA2>) {
    loop {
        if SAIDA_STATE.load(Ordering::Relaxed) {
            saida.set_high(); 
        } else {
            saida.set_low(); 
        }
        Timer::after(Duration::from_millis(500)).await; 
    }
}
#[embassy_executor::task]
async fn tarefa_entrada(mut entrada: ExtiInput<'static, PA4>) {
    loop {
        entrada.wait_for_falling_edge().await;
        let current_state = SAIDA_STATE.load(Ordering::Relaxed);
        SAIDA_STATE.store(!current_state, Ordering::Relaxed);
        entrada.wait_for_rising_edge().await;
    }
}

#[embassy_executor::main]
async fn main(spawner: Spawner) {
    let p = embassy_stm32::init(Default::default());

    let saida = Output::new(p.PA2, Level::Low, Speed::Low); 
    let saida2 = Output::new(p.PA1, Level::Low, Speed::Low);

    let entrada = Input::new(p.PA4, Pull::Up); 
    let entrada_exti = ExtiInput::new(entrada, p.EXTI4); 

    spawner.spawn(tarefa_entrada(entrada_exti)).unwrap(); 
    spawner.spawn(tarefa_um(saida2)).unwrap();    
    spawner.spawn(tarefa_saida(saida)).unwrap(); 
}