#include "chip_8.h"

int main(int argc, char* argv[]) {
	if(argc < 2) {
		printf("ERROR: Pass game ROM file.\n");
		return 1;
	}

	if(load_program(argv[1]) != 0) {
		return 1;
	}

	if(init_video() != 0) {
		return 1;
	}
    
    if(init_core() != 0) {
        return 1;
    }

	run();

    close_video();

	return 0;
}


void run() {
    SDL_Event e;
	while(true) {
        while(SDL_PollEvent( &e ) != 0) {
            if(e.type == SDL_QUIT) {
                return;
            }
        }

		SDL_Delay(2);

		u_int16_t opcode = fetch();
		decode(opcode);
	}
}