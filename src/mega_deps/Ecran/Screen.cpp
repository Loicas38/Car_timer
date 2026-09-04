#include "Screen.hpp"

// Initialisation directe du membre lcd via la liste d'initialisation
Screen::Screen() 
    : m_escape_display(9), // to be changed
      m_select(5),
      m_left_swipe(4),
      m_right_swipe(6),
      wait_slip(300),
      lcd(rs, en, d4, d5, d6, d7) 
{
    lcd.begin(16, 2);
}


Screen::Screen(int select_code, int escape_code) 
    : m_escape_display(escape_code), // to be changed
      m_select(select_code),
      wait_slip(500),
      lcd(rs, en, d4, d5, d6, d7) 
{
    lcd.begin(16, 2);
}


void Screen::slip_text_one_line(int line, int col, int* deb_id, String content){
    lcd.setCursor(col, line);
    lcd.print(content.substring(*deb_id));
}


void Screen::display(String line1, String line2){
    // The id of the first char which has to be displayed
    int deb1 = 0;
    int deb2 = 0;

    lcd.clear();
    lcd.setCursor(0, 0);
    
    uint32_t wait1_time = millis();
    uint32_t wait2_time = millis();

    // wether it is needed or not to wait the 1.5 sec (begin or end of line) without sliding
    bool wait1 = true;
    bool wait2 = true;

    while(true){
        // When the text can be fully printed on the line
        if(line1.length() <= 16){
            deb1 = 0;
        }

        if(line2.length() <= 16){
            deb2 = 0;
        }


        //Serial.println("loop");
        lcd.clear();
        lcd.setCursor(0, 0);
        
        lcd.print(line1.substring(deb1, deb1 + 16));

        if(wait1){
            if(millis() - wait1_time > 1500){
                if(deb1 == 0){
                    deb1 = 1;
                    wait1 = false;
                } else {
                    deb1 = 0;
                    wait1_time = millis();
                }
            } 
        } else {
            deb1++;
            if(deb1 + 16 > line1.length()){
                deb1--;
                wait1 = true;
                wait1_time = millis();
            }
        }
        

        lcd.setCursor(0, 1);
        
        lcd.print(line2.substring(deb2, deb2 + 16));

        if(wait2){
            if(millis() - wait2_time > 1500){
                if(deb2 == 0){
                    deb2 = 1;
                    wait2 = false;
                } else {
                    deb2 = 0;
                    wait2_time = millis();
                }
            } 
        } else {
            deb2++;
            if(deb2 + 16 > line2.length()){
                deb2--;
                wait2 = true;
                wait2_time = millis();
            }
        }

        if (waitClickTouche(m_escape_display, wait_slip)){
            return;
        }  

    }   
}


void Screen::display(String line1, String line2, uint32_t duration) {
    // The id of the first char which has to be displayed
    int deb1 = 0;
    int deb2 = 0;

    lcd.clear();
    lcd.setCursor(0, 0);
    
    uint32_t wait1_time = millis();
    uint32_t wait2_time = millis();

    // wether it is needed or not to wait the 1.5 sec (begin or end of line) without sliding
    bool wait1 = true;
    bool wait2 = true;


    unsigned long deb_time = millis();

    while(duration == 0 || millis() - deb_time < duration){
        if (duration == 0){
            duration++;
        }

        // When the text can be fully printed on the line
        if(line1.length() <= 16){
            deb1 = 0;
        }

        if(line2.length() <= 16){
            deb2 = 0;
        }

        Serial.println("loop");
        lcd.clear();
        lcd.setCursor(0, 0);
        
        lcd.print(line1.substring(deb1, deb1 + 16));

        if(wait1){
            if(millis() - wait1_time > 1500){
                if(deb1 == 0){
                    deb1 = 1;
                    wait1 = false;
                } else {
                    deb1 = 0;
                    wait1_time = millis();
                }
            } 
        } else {
            deb1++;
            if(deb1 + 16 > line1.length()){
                deb1--;
                wait1 = true;
                wait1_time = millis();
            }
        }

        lcd.setCursor(0, 1);
        
        lcd.print(line2.substring(deb2, deb2 + 16));

        if(wait2){
            if(millis() - wait2_time > 1500){
                if(deb2 == 0){
                    deb2 = 1;
                    wait2 = false;
                } else {
                    deb2 = 0;
                    wait2_time = millis();
                }
            } 
        } else {
            deb2++;
            if(deb2 + 16 > line2.length()){
                deb2--;
                wait2 = true;
                wait2_time = millis();
            }
        }

        if (waitClickTouche(m_escape_display, wait_slip)){
            Serial.println("over return");
            return;
        }   
    }

    Serial.println("over");
}


void Screen::flashing_display(String line1, String line2){
    while(true){
        display(line1, line2, 800);
        

        if (waitClickTouche(m_escape_display, 400)){
            return;
        }
    }
}

void Screen::flashing_display(String line1, String line2, uint32_t duration){
    uint32_t deb = millis();

    while(millis() - deb < duration){
        display(line1, line2, 600);
        
        lcd.clear();

        if (waitClickTouche(m_escape_display, 300)){
            return;
        }
    }
}


int Screen::menu(String question, String* options, int nb_options){
    int deb_quest = 0;
    int deb_opt = 0;
    int select_id = 0;

    while(true){
        lcd.clear();
        slip_text_one_line(0, 0, &deb_quest, question);

        slip_text_one_line(0, 0, &deb_opt, options[select_id]);

        int touch = waitAndGetTouche(wait_slip);

        if (touch == m_right_swipe){
            select_id++;
            if (select_id >= nb_options){
                select_id = 0;
            }
        }

        else if (touch == m_left_swipe){
            select_id--;
            if (select_id < 0){
                select_id = nb_options - 1;
            }
        }

        else if (touch == m_select){
            return select_id;
        }

        else if (touch == m_escape_display){
            return -1;
        }

        else {
            deb_opt++;
        }

        deb_quest++;
    }
}

bool* Screen::multiple_options(String question, String* options, int nb_options){
    bool* res = (bool*)malloc(sizeof(bool)*nb_options);

    for(int i = 0; i < nb_options; i++){
        res[i] = false;
    }

    int disp_opt = 0;

    int deb_quest = 0;
    int deb_opt = 0;

    while (true){
        lcd.clear();

        slip_text_one_line(0, 0, &deb_quest, question);
        
        if (res[disp_opt]){
            lcd.setCursor(0, 1);
            // Another char may be better
            lcd.print("O");
        }

        slip_text_one_line(1, 1, &disp_opt, options[disp_opt]);

        int touch = waitAndGetTouche(wait_slip);

        if (touch == m_escape_display){
            return res;
        }

        else if (touch == m_select){
            res[disp_opt] = !disp_opt;
        }

        else if (touch == -1){
            deb_opt++;
        }

        else if (m_left_swipe == touch){
            disp_opt += 1;
            if (disp_opt >= nb_options){
                disp_opt = 0;
            }
        }

        else if (m_right_swipe == touch){
            disp_opt--;
            if (disp_opt < 0){
                disp_opt = nb_options - 1;
            }
        }

        deb_quest++;
    }
}

void Screen::chose_select(int new_select){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press sel. touch");
    
    m_select = waitAndGetTouche();
}

void Screen::chose_escape(int new_select){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press esc. touch");
    
    m_escape_display = waitAndGetTouche();
}


String Screen::build_string(String message){
    String chaine = "";
    int letter = 65;

    while(true){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(message);

        lcd.setCursor(0, 1);
        lcd.print(chaine);
        lcd.setCursor(chaine.length(), 1);
        lcd.print((char)letter);

        int touch = waitAndGetTouche();

        if (touch == m_escape_display){
            return chaine;
        }

        else if(touch == m_select){
            chaine = chaine + (char)letter;
        }

        else if (touch == m_left_swipe){
            letter--;

            if(letter < 48){
                letter = 122;
            } else if (letter < 65 && letter > 57){
                letter = 57;
            } else if (letter < 97 && letter > 90){
                letter = 90;
            }
        }

        else if (touch == m_right_swipe){
            letter ++;

            if (letter > 122){
                letter = 48;
            } else if(letter > 90 && letter < 97){
                letter = 97;
            } else if (letter > 57 && letter < 65){
                letter = 65;
            }
        }
    }
}


void Screen::display_once(String line1, String line2){
    //lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}



int Screen::double_line_menu(String options[][2], int nb_options){
    int pos = 0;
    Serial.println("into menu");

    while(true){
        lcd.clear();
        display_once(options[pos][0], options[pos][1]);

        int touch = waitAndGetTouche();

        if(touch == m_escape_display){
            return -1;
        }

        if(touch == m_select){
            return pos;
        }

        if (touch == m_left_swipe){
            pos--;
            if (pos < 0){
                pos = nb_options - 1;
            }
        }

        if (touch == m_right_swipe){
            pos++;
            if(pos >= nb_options){
                pos = 0;
            }
        }
    }
}