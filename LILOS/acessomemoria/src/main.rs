#![no_std]
#![no_main]

use cortex_m_rt::entry;
use lilos::exec::{run_tasks};
use stm32g0xx_hal as hal;
use hal::prelude::*;
use hal::stm32;
use panic_halt as _; 

#[entry]
fn main() -> ! {
    let dp = stm32::Peripherals::take().unwrap();
    let mut rcc = dp.RCC.freeze(hal::rcc::Config::hsi(hal::rcc::Prescaler::Div2));

    let gpioa = dp.GPIOA.split(&mut rcc);
    let mut led1 = gpioa.pa1.into_push_pull_output();
    let mut led2 = gpioa.pa2.into_push_pull_output();

    
    let first_task = core::pin::pin!(async {
        let mut array: [i32; 10] = [0; 10];
        let index = 10; 

        if index >= array.len() {
            led1.set_high().unwrap();
        } else {
            led2.set_high().unwrap();
        }

        
        array[index] = 42; 

    });

      
    run_tasks(&mut [first_task], lilos::exec::ALL_TASKS,);

    
}
