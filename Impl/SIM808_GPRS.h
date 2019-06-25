#ifndef __DFROBOT_SIM808_GPRS_H__
#define __DFROBOT_SIM808_GPRS_H__

class DFRobot_SIM808_GPRS
{
public:    


    /** call someone
     *  @param  number  the phone number which you want to call
     *  @returns
     *      true on success
     *      false on error
     */
    bool callUp(char* number);

    /** auto answer if coming a call
     *  @returns
     */    
    void answer(void);
    
    /** hang up if coming a call
     *  @returns
     *      true on success
     *      false on error
     */    
    bool hangup(void);  

    /** Disable +CLIP notification when an incoming call is active, RING text is always shown. See isCallActive function
     *  This is done in order no to overload serial outputCheck if there is a call active and get the phone number in that case
     *  @returns
     *      true on success
     *      false on error
     */
    bool disableCLIPring(void);
	
	/** Get Subscriber Number (your number) using AT+CNUM command, but if nothing returns, then
	 *  you need to command this to your SIM900. (See AT+CPBS, AT+CPBW)
	 *	AT+CPBS="ON"
	 *	AT+CPBW=1,"+{Your Number}",145
	 *	AT+CPBS="SM"
	 *  @param
	 *	@return
	 *		true on success
	 *		false on error
	 */
	bool getSubscriberNumber(char *number);
    
	/** Check if there is a call active and get the phone number in that case
     *  @returns
     *      true on success
     *      false on error
     */
    bool isCallActive(char *number);  

    /** get DateTime from SIM900 (see AT command: AT+CLTS=1) as string
     *  @param
     *  @returns
     *      true on success
     *      false on error
     * 
     * 	   If it doesn't work may be for two reasons:
     *	 	1. Your carrier doesn't give that information
     *		2. You have to configurate the SIM900 IC.
         *			- First with SIM900_Serial_Debug example try this AT command: AT+CLTS?
	 *			- If response is 0, then it is disabled.
	 *			- Enable it by: AT+CLTS=1
	 *			- Now you have to save this config to EEPROM memory of SIM900 IC by: AT&W
	 *			- Now, you have to power down and power up again the SIM900 
	 *			- Try now again: AT+CCLK?
	 *			- It should work now
	 *
     */
       
    bool getDateTime(char *buffer);
    
	/** get Signal Strength from SIM900 (see AT command: AT+CSQ) as integer
	*  @param
	*  @returns
	*      true on success
	*      false on error
	*/
	bool getSignalStrength(int *buffer);
    
    /** Send USSD Command Synchronously (Blocking call until unsolicited response is received)
     *  @param
	 *		*ussdCommand string command UUSD, ex: *123#
	 *		*resultCode	char Result Code, see AT+CUSD command
	 *		*response	string response
	 *		*cellBroadcast	int Cell Broadcast Data Coding Scheme
     *  @returns
     *      true on success
     *      false on error
     */  
	bool sendUSSDSynchronous(char *ussdCommand, char *resultcode, char *response);

    /** Cancel USSD Session
     *  @returns
     *      true on success cancel active session
     *      false on error or because no active session
     */
	bool cancelUSSDSession(void);


    bool getLocation(const char* apn, char* latitude, char* longitude);
};
#endif
