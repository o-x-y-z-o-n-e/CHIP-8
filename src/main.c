#include "chip_8.h"

bool loaded = false;
bool replay;

int main(int argc, char* argv[]) {
	for (int i = 1; i < argc; i++) {
		if (check_arg(argv[i], "-v", "-version")) {
			printf("CHIP-8 Version: %s\n", VERSION);
			return 0;
		}

		if (load_program(argv[1]) != 0) {
			return 1;
		}

		loaded = true;
	}

	if(!loaded) {
		printf("ERROR: Pass game ROM file.\n");
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
	replay = false;

	int counter = 0;

    SDL_Event e;
	while(true) {
        while(SDL_PollEvent( &e ) != 0) {
            if(e.type == SDL_QUIT) {
                return;
			}
			else if (e.type == SDL_KEYDOWN) {
				handle_key(e.key.keysym.sym, true);
			}
			else if (e.type == SDL_KEYUP) {
				handle_key(e.key.keysym.sym, false);
			}
        }

		SDL_Delay(2);
		counter += 2;

		if (replay) {
			rollback();
			replay = false;
		} else {
			if (counter > 16) {
				dec_delay();
				dec_sound();
				counter = 0;
			}

			u_int16_t opcode = fetch();
			decode(opcode);
		}
	}
}

void halt() {
	replay = true;
}

void handle_key(SDL_Keycode key, bool state) {
	switch (key) {
		case SDLK_1: {
			set_key(0x1, state);
			break;
		}

		case SDLK_2: {
			set_key(0x2, state);
			break;
		}

		case SDLK_3: {
			set_key(0x3, state);
			break;
		}

		case SDLK_4: {
			set_key(0xC, state);
			break;
		}

		case SDLK_q: {
			set_key(0x4, state);
			break;
		}

		case SDLK_w: {
			set_key(0x5, state);
			break;
		}

		case SDLK_e: {
			set_key(0x6, state);
			break;
		}

		case SDLK_r: {
			set_key(0xD, state);
			break;
		}

		case SDLK_a: {
			set_key(0x7, state);
			break;
		}

		case SDLK_s: {
			set_key(0x8, state);
			break;
		}

		case SDLK_d: {
			set_key(0x9, state);
			break;
		}

		case SDLK_f: {
			set_key(0xE, state);
			break;
		}

		case SDLK_z: {
			set_key(0xA, state);
			break;
		}

		case SDLK_x: {
			set_key(0x0, state);
			break;
		}

		case SDLK_c: {
			set_key(0xB, state);
			break;
		}

		case SDLK_v: {
			set_key(0xF, state);
			break;
		}
	}
}


bool check_arg(const char* arg, const char* short_arg, const char* long_arg) {
	if (strcmp(arg, short_arg) == 0) return true;
	if (strcmp(arg, long_arg) == 0) return true;

	return false;
}