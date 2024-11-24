#![no_std]  
#![no_main]  

use panic_halt as _;  
use stm32g0xx_hal as hal;  
use hal::exti::Event;  
use hal::gpio::*;  
use hal::pac;  
use hal::prelude::*;  
use hal::rcc::{Config, Prescaler};  
use hal::stm32;  
use rtic::app;  
use systick_monotonic::{fugit, Systick};  
use core::sync::atomic::{AtomicBool, Ordering};  

#[app(device = stm32g0xx_hal::pac, peripherals = true, dispatchers = [SPI1])]  
mod app {  
    use super::*;  

    #[shared]  
    struct Shared {  
        saida_state: AtomicBool, 
    }  

    #[local]  
    struct Local {  
        exti: stm32::EXTI,  
        saida: PA2<Output<PushPull>>,  
        saida2: PA1<Output<PushPull>>,  
    }  

    #[monotonic(binds = SysTick, default = true)]  
    type MyMono = Systick<1000>;  

    #[init]  
    fn init(cx: init::Context) -> (Shared, Local, init::Monotonics) {  
        let dp: pac::Peripherals = cx.device;  
        let mut rcc = dp.RCC.freeze(Config::hsi(Prescaler::Div2));  

        let gpioa = dp.GPIOA.split(&mut rcc);  

        let mut exti = dp.EXTI;  

        let saida = gpioa.pa2.into_push_pull_output();  
        let saida2 = gpioa.pa1.into_push_pull_output();  

        gpioa.pa4.listen(SignalEdge::Falling, &mut exti);  

        let systick = Systick::new(cx.core.SYST, 8_000_000);  

        tarefa_um::spawn().ok();

        (  
            Shared { saida_state: AtomicBool::new(false) }, 
            Local { exti, saida, saida2 },  
            init::Monotonics(systick),  
        )  
    }  
    #[task(local = [saida2])]  
    fn tarefa_um(cx: tarefa_um::Context) {  
        cx.local.saida2.toggle().unwrap();  
        tarefa_um::spawn_after(fugit::Duration::<u64, 1, 1000>::millis(500)).ok();  
    }  

    
    #[task(binds = EXTI4_15, local = [exti], shared = [saida_state])]  
    fn tarefa_entrada(mut cx: tarefa_entrada::Context) { 
        cx.shared.saida_state.lock(|saida_state_guard| {  
            let new_state = saida_state_guard.load(Ordering::Relaxed);  
            saida_state_guard.store(!new_state, Ordering::Relaxed);  
        });  

        cx.local.exti.unpend(Event::GPIO4);  
        tarefa_saida::spawn().ok();  
    }  
    #[task(local = [saida], shared = [saida_state])]  
    fn tarefa_saida(mut cx:  tarefa_saida::Context) { 
        cx.shared.saida_state.lock(|saida_state_guard| {  
            if saida_state_guard.load(Ordering::Relaxed) {  
                cx.local.saida.set_high().unwrap(); 
            } else {  
                cx.local.saida.set_low().unwrap();   
            }  
        });  
        tarefa_saida::spawn_after(fugit::Duration::<u64, 1, 1000>::millis(500)).ok(); 
    }  
}