cmake_minimum_required(VERSION 3.6)

include(pico_sdk_import.cmake)

project(pico2_blinky C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})


pico_sdk_init()

#nome del file eseguibile generato
add_executable(pico2_blinky) 

#dichiaro che c'è un programma PIO, chiamato new_scan.pio, che voglio
#buildare come HEADER per il mio file c, per essere buildato
#nell'eseguibile scan_pio(come detto prima)
pico_generate_pio_header(pico2_blinky ${CMAKE_CURRENT_LIST_DIR}/blink.pio)


#listo i file source per il programma scan_pio

target_sources(pico2_blinky PRIVATE main.c)

#includo le librerie dei vari API per poter usare le funzioni specifiche 
#nel programma C
target_link_libraries(pico2_blinky PRIVATE
	pico_stdlib
	hardware_pio
	hardware_clocks
	
	)
pico_enable_stdio_usb(pico2_blinky 1)
pico_enable_stdio_uart(pico2_blinky 0)
#il compilatore returna solo un eseguibile, gli dico di farmi anche un .uf2
#(usb to file) per drag and drop nel pico
pico_add_extra_outputs(pico2_blinky)
