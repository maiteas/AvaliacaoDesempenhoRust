#![no_std]
#![no_main]

use embassy_executor::Spawner;
use embassy_stm32::gpio::{Level, Output, Speed};
use embassy_stm32::peripherals::{PA1, PA2, PA3};
use embassy_sync::blocking_mutex::raw::ThreadModeRawMutex;
use embassy_sync::channel::{Channel, Sender, Receiver};
use embassy_time::{Duration, Timer};
use panic_halt as _;


#[embassy_executor::task]
async fn tarefa_um(sender: Sender<'static, ThreadModeRawMutex, u8, 64>, mut saida: Output<'static, PA1>) {
    let mut value = 0;
    loop {
        
        value = if value == 1 { 0 } else { 1 };

        
        if value == 1 {
            saida.set_high();
        } else {
            saida.set_low(); 
        }

        
        sender.send(value).await; 
        
        
        Timer::after(Duration::from_millis(900)).await;
    }
}


#[embassy_executor::task]
async fn tarefa_dois(receiver: Receiver<'static, ThreadModeRawMutex, u8, 64>, sender: Sender<'static, ThreadModeRawMutex, u8, 64>, mut saida: Output<'static, PA2>) {
    loop {
        
        let value = receiver.receive().await; 
        if value == 1 {
            saida.set_high();
        } else {
            saida.set_low();
        }
        
        sender.send(value).await;

        
        Timer::after(Duration::from_millis(1000)).await;
    }
}


#[embassy_executor::task]
async fn tarefa_tres(receiver: Receiver<'static, ThreadModeRawMutex, u8, 64>, mut saida: Output<'static, PA3>) {
    loop {
        
        let value = receiver.receive().await; 

        
        if value == 1 {
            saida.set_high(); 
        } else {
            saida.set_low(); 
        }

        // Atraso de 1300ms
        Timer::after(Duration::from_millis(1300)).await;
    }
}


#[embassy_executor::main]
async fn main(spawner: Spawner) {
    
    // Inicializa os periféricos do STM32
    let p = embassy_stm32::init(Default::default());

    
    static FILA_1: Channel<ThreadModeRawMutex, u8, 64> = Channel::new();
    static FILA_2: Channel<ThreadModeRawMutex, u8, 64> = Channel::new();

    // Inicializa as saídas nos pinos PA1, PA2 e PA3
    let saida1 = Output::new(p.PA1, Level::Low, Speed::Medium);
    let saida2 = Output::new(p.PA2, Level::Low, Speed::Medium);
    let saida3 = Output::new(p.PA3, Level::Low, Speed::Medium);

    
    spawner.spawn(tarefa_um(FILA_1.sender(), saida1)).unwrap();
    spawner.spawn(tarefa_dois(FILA_1.receiver(), FILA_2.sender(), saida2)).unwrap();
    spawner.spawn(tarefa_tres(FILA_2.receiver(), saida3)).unwrap();
}
