#![no_std]
#![no_main]

use embassy_executor::Spawner;
use embassy_stm32::gpio::{Level, Output, Speed};
use embassy_stm32::peripherals::{PA1, PA2, PA3, PA4};
use embassy_time::{Duration, Timer};
use embassy_sync::blocking_mutex::raw::ThreadModeRawMutex;
use embassy_sync::mutex::Mutex;
use panic_halt as _;




static SAIDA1: Mutex<ThreadModeRawMutex, Option<Output<'static, PA1>>> = Mutex::new(None);
static SAIDA2: Mutex<ThreadModeRawMutex, Option<Output<'static, PA2>>> = Mutex::new(None);
static SAIDA3: Mutex<ThreadModeRawMutex, Option<Output<'static, PA3>>> = Mutex::new(None);
static SAIDA4: Mutex<ThreadModeRawMutex, Option<Output<'static, PA4>>> = Mutex::new(None);

#[embassy_executor::task]
async fn tarefa_um() {
    let mut saida1 = SAIDA1.lock().await;
    let mut saida2 = SAIDA2.lock().await;
    let mut saida3 = SAIDA3.lock().await;
    let mut saida4 = SAIDA4.lock().await;

    saida1.as_mut().unwrap().set_high();
    saida2.as_mut().unwrap().set_low();
    saida3.as_mut().unwrap().set_low();
    saida4.as_mut().unwrap().set_low();
    Timer::after(Duration::from_millis(1800)).await;
}

#[embassy_executor::task]
async fn tarefa_dois() {
    let mut saida1 = SAIDA1.lock().await;
    let mut saida2 = SAIDA2.lock().await;
    let mut saida3 = SAIDA3.lock().await;
    let mut saida4 = SAIDA4.lock().await;

    saida2.as_mut().unwrap().set_high();
    saida1.as_mut().unwrap().set_low();
    saida3.as_mut().unwrap().set_low();
    saida4.as_mut().unwrap().set_low();
    Timer::after(Duration::from_millis(1800)).await;
}

#[embassy_executor::task]
async fn tarefa_tres() {
    let mut saida1 = SAIDA1.lock().await;
    let mut saida2 = SAIDA2.lock().await;
    let mut saida3 = SAIDA3.lock().await;
    let mut saida4 = SAIDA4.lock().await;

    saida3.as_mut().unwrap().set_high();
    saida1.as_mut().unwrap().set_low();
    saida2.as_mut().unwrap().set_low();
    saida4.as_mut().unwrap().set_low();
    Timer::after(Duration::from_millis(1800)).await;
}

#[embassy_executor::task]
async fn tarefa_quatro() {
    let mut saida1 = SAIDA1.lock().await;
    let mut saida2 = SAIDA2.lock().await;
    let mut saida3 = SAIDA3.lock().await;
    let mut saida4 = SAIDA4.lock().await;

    saida4.as_mut().unwrap().set_high();
    saida1.as_mut().unwrap().set_low();
    saida2.as_mut().unwrap().set_low();
    saida3.as_mut().unwrap().set_low();
    Timer::after(Duration::from_millis(3900)).await;
}

#[embassy_executor::main]
async fn main(spawner: Spawner) {
    let p = embassy_stm32::init(Default::default());

    let saida1 = Output::new(p.PA1, Level::Low, Speed::Low);
    let saida2 = Output::new(p.PA2, Level::Low, Speed::Low);
    let saida3 = Output::new(p.PA3, Level::Low, Speed::Low);
    let saida4 = Output::new(p.PA4, Level::Low, Speed::Low);

    *SAIDA1.lock().await = Some(saida1);
    *SAIDA2.lock().await = Some(saida2);
    *SAIDA3.lock().await = Some(saida3);
    *SAIDA4.lock().await = Some(saida4);

    loop {
        spawner.spawn(tarefa_um()).unwrap();
        Timer::after(Duration::from_millis(1800)).await;

        spawner.spawn(tarefa_dois()).unwrap();
        Timer::after(Duration::from_millis(1800)).await;

        spawner.spawn(tarefa_tres()).unwrap();
        Timer::after(Duration::from_millis(1800)).await;

        spawner.spawn(tarefa_quatro()).unwrap();
        Timer::after(Duration::from_millis(1800)).await;
    }
}