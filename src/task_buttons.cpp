#include "task_buttons.hpp"


extern boolean newCommand;
extern uint8_t valCommand;


/**
 * @brief 
 * 
 * @param pvParameters 
 */
void TaskButtons(void *pvParameters)
{
    (void) pvParameters;

    for (;;)
    {
        try
        {     
            if(digitalRead(PIN_BTN_OPEN) == LOW)
            {
                if(newCommand == false) { valCommand = 1; newCommand = true; }
            } 

            if(digitalRead(PIN_BTN_CLOSE) == LOW)
            {
                if(newCommand == false) { valCommand = 2; newCommand = true; }
            } 
            
            if(digitalRead(PIN_BTN_ADV_FAST) == LOW)
            {
                if(newCommand == false) { valCommand = 3; newCommand = true; }
            } 
            
            if(digitalRead(PIN_BTN_ADV_LOW) == LOW)
            {
                if(newCommand == false) { valCommand = 4; newCommand = true; }
            } 

        }
        catch (const std::exception& e)
        { 

        }

        vTaskDelay(50);
    }
}