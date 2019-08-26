#pragma once

class DFRobot_SIM808_SMS
{
public:
    /** send text SMS
     *  @param  *number phone number which SMS will be send to
     *  @param  *data   message that will be send to
     *  @returns
     *      false on success
     *      true on error
     */
    bool sendSMS(const char* number, const char* data);

    /** Check if there is any UNREAD SMS: this function DOESN'T change the UNREAD status of the SMS
     *  @returns
     *      1..20 on success, position/index where SMS is stored, suitable for the function ReadSMS
     *      -1 on error
     *       0 - there is no SMS with specified status (UNREAD)
     */

	char isSMSunread();
    
    /** read SMS, phone and date if getting a SMS message. It changes SMS status to READ 
     *  @param  messageIndex  SIM position to read
     *  @param  message  buffer used to get SMS message
     *  @param  length  length of message buffer
     *  @param  phone  buffer used to get SMS's sender phone number
     *  @param  datetime  buffer used to get SMS's send datetime
     *  @returns
     *      true on success
     *      false on error
     */
    bool readSMS(int messageIndex, char *message, int length, char *phone, char *datetime); 

    /** read SMS if getting a SMS message
     *  @param  buffer  buffer that get from DFRobot_SIM808 module(when getting a SMS, DFRobot_SIM808 module will return a buffer array)
     *  @param  message buffer used to get SMS message
     *  @param  check   whether to check phone number(we may only want to read SMS from specified phone number)
     *  @returns
     *      true on success
     *      false on error
     */
    bool readSMS(int messageIndex, char *message, int length);

    /** delete SMS message on SIM card
     *  @param  index   the index number which SMS message will be delete
     *  @returns
     *      true on success
     *      false on error
     */
    bool deleteSMS(int index);
};