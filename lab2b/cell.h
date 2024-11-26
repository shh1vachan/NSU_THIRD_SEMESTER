#ifndef CELL_H
#define CELL_H

class cell 
{
public:
    cell();

    bool is_alive() const;
    void set_current_state(bool state);
    void save_state(bool state);
    void update_state();

private:
    bool current_state;     
    bool next_state;       
};

#endif // CELL_H
