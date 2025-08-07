cmake_minimum_required(VERSION 3.6)
set(PICO_PLATFORM rp2350-arm-s)
include(pico_sdk_import.cmake)

project(pico-60 C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})


pico_sdk_init()

#nome del file eseguibile generato
add_executable(pico-60) 

#dichiaro che c'è un programma PIO, chiamato new_scan.pio, che voglio
#buildare come HEADER per il mio file c, per essere buildato
#nell'eseguibile scan_pio(come detto prima)
pico_generate_pio_header(pico-60 ${CMAKE_CURRENT_LIST_DIR}/pico.pio)


#listo i file source per il programma scan_pio

target_sources(pico-60 PRIVATE main.c)

#includo le librerie dei vari API per poter usare le funzioni specifiche 
#nel programma C
target_link_libraries(pico-60 PRIVATE
	pico_stdlib
	hardware_pio
	hardware_clocks
	hardware_adc
	hardware_pwm	
	)
pico_enable_stdio_usb(pico-60 1)
pico_enable_stdio_uart(pico-60 0)
#il compilatore returna solo un eseguibile, gli dico di farmi anche un .uf2
#(usb to file) per drag and drop nel pico
pico_add_extra_outputs(pico-60)
