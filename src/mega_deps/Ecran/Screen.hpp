#include <Arduino.h>
#include <LiquidCrystal.h>
#include "mega_deps/InfraRouge/InfraRouge.h"

#ifndef ECRAN
#define ECRAN

class Screen{

private:
    // if this command is pressed on the telecomand, then the display ends
    int m_escape_display;
    // if there is something to select, this command will be used
    int m_select;
    int m_left_swipe;
    int m_right_swipe;

    // the time before switching when there is a text which is too long to be displayed on the screen
    int wait_slip;

    // for the screen connection
    static constexpr int rs = 41, en = 43, d4 = 47, d5 = 49, d6 = 40, d7 = 45;
    LiquidCrystal lcd;

    // writes content on the line number line, beginning on the col colun.
    // It starts at the deb_id of content
    void slip_text_one_line(int line, int col, int* deb_id, String content);


public:
    Screen();
    Screen(int select_code, int escape_code);

    // displays the content until the escape touch is pressed
    void display(String line1, String line2);
    // duration : the time to display the message in milliseconds
    void display(String line1, String line2, uint32_t duration);
    void flashing_display(String line1, String line2);
    void flashing_display(String line1, String line2, uint32_t duration);

    // Displays the text on each line, and returns immediately once it is finished
    // DOESN'T CLEAR THE SCREEN
    void display_once(String line1, String line2);

    // returns the selected option
    int menu(String question, String* options, int nb_options);
    // a menu in which both lines are used, so there is no first line question
    int double_line_menu(String options[][2], int nb_options);
    // Allows the user to select multiple options. Returns a list of booleans, 
    // with true for the elements whch were chosen, false for the others
    bool* multiple_options(String question, String* options, int nb_options);


    void chose_select(int new_select);
    void chose_escape(int new_escape);

    // Displays the message on the first line and allows the user to input a string on the second line
    String build_string(String message);
};

#endif