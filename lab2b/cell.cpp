#include "cell.h"

cell::cell() : current_state(false), next_state(false) {}

bool cell::is_alive() const {return current_state;}
void cell::set_current_state(bool state) {current_state = state;}
void cell::save_state(bool state) {next_state = state;}
void cell::update_state() {current_state = next_state;}  
