#include <stdlib.h>
#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"

#include "blink.pio.h"
//todo fixare VOICES NELLA FUNZIONE RISCHIO ESPLOSIONE
#define LED_PIN 25
#define SET_PIN 13
#define IN_PIN 10
#define NUM_VOICES 6
//pio0 ha 3 sm di scan e 1 di output
//pio1 ha 4 sm di out
//pio2 ha 1 sm di out
//note da c1 A c6
PIO pio = pio0;
uint sm = 0;
float voice_to_frequency[NUM_VOICES] = {0,0,0,0,0,0}; //frequency of each voice, 0 is voice not used
uint8_t voice_to_gate[NUM_VOICES] = {0,0,0,0,0,0}; //gate for each voice 0 off 1 on
const uint8_t VOICE_TO_PIO[NUM_VOICES] = {0,1,1,1,1,2};
const uint8_t VOICE_TO_SM[NUM_VOICES] = {3,0,1,2,3,0};
const uint8_t VOICE_TO_GATE_PIN[NUM_VOICES]= {0,0,0,0,0,0};
float VOICES[NUM_VOICES];
uint8_t curr_voice;//segnalino per round robin NON CI CAPISCO PIU U N CAZZO
const float note_freq[61] = {
	32.7032, 34.6478, 36.7081, 38.8909, 41.2034, 43.6535, 46.2493, 48.9994, 51.9131, 55.0000, 58.2705, 61.7354, // C1 - B1
	65.4064, 69.2957, 73.4162, 77.7817, 82.4069, 87.3071, 92.4986, 97.9989,103.826, 110.000,116.541,123.471, // C2 - B2
	130.813,138.591,146.832,155.563,164.814,174.614,184.997,195.998,207.652,220.000,233.082,246.942, // C3 - B3
	261.626,277.183,293.665,311.127,329.628,349.228,369.994,391.995,415.305,440.000,466.164,493.883, // C4 - B4
	523.251,554.365,587.330,622.254,659.255,698.456,739.989,783.991,830.609,880.000,932.328,987.767, // C5 - B5
	1046.50                                                   // C6
};
void libera_voce_con_freq(float freq){
	for(int indice = 0; indice < NUM_VOICES;indice++){
		if (voice_to_gate[indice] && voice_to_frequency[indice]==freq){
			voice_to_gate[indice] = 0;
			voice_to_frequency[indice] = 0.0f;
			//pio_sm_put(VOICE_TO_PIO[indice],VOICE_TO_SM[indice],0);
			//gpio_put(VOICE_TO_GATE_PINS[indice],0);
		}
	}
}
void assegna_voce(float freq){
	voice_to_frequency[curr_voice] = freq;
	voice_to_gate[curr_voice] = 1;
	//pio_sm_put(VOICE_TO_PIO[curr_voice],VOICE_TO_SM[curr_voice],freq * pitch_detune);
	curr_voice = (curr_voice + 1) % NUM_VOICES;
}
void keyboard_task(uint64_t prev_scan,uint64_t curr_scan){
		
	uint64_t premuti = curr_scan & ~prev_scan;
	uint64_t rilasciati = ~curr_scan & prev_scan;	
	
	while(rilasciati){
		uint i = __builtin_ctzll(rilasciati);
		float freq = note_freq[i];
		libera_voce_con_freq(freq);
		rilasciati &=  ~(1ULL<<i);	
	}
	
	while(premuti){
		uint i = __builtin_ctzll(rilasciati);
		float freq = note_freq[i];
		assegna_voce(freq);
		premuti &= ~(1ULL<<i);
	}

		
}
int main(){
	stdio_init_all();//per comunicazione usb
	setup_default_uart();
	//prima fai scan per adc
	uint offset = pio_add_program(pio,&blink_program);
	blink_program_init(pio,sm,offset,SET_PIN,IN_PIN,2000);
	sleep_ms(1000);
	printf("Starting program\n");
	uint32_t prev = 0;
	uint32_t data;
	while(1){
		data = pio_sm_get(pio,sm);
		keyboard_task(prev,data);
		printf("num: %u VOICES[",data);
	
		for(int i = 0;i<NUM_VOICES;i++){
				printf("%f ,",voice_to_frequency[i]);

		}
		printf("]\n");
		for(int j = 0;j<NUM_VOICES;j++){
				printf("%f ,",voice_to_gate[j]);

		}
		printf("]\n");
		printf("current voice: %u\n",curr_voice);
		sleep_ms(100);
	}
	return 0;
}
