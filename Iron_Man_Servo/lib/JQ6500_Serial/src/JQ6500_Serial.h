/** 
 * Arduino Library for JQ6500 MP3 Module
 * 
 * Copyright (C) 2014-2021 James Sleeman, <http://sparks.gogo.co.nz/jq6500/index.html>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 * 
 * @author James Sleeman, http://sparks.gogo.co.nz/
 * @license MIT License
 * @file
 */

// Please note, the Arduino IDE is a bit retarded, if the below define has an
// underscore other than _h, it goes mental.  Wish it wouldn't  mess
// wif ma files!
#ifndef JQ6500Serial_h
#define JQ6500Serial_h


#define MP3_EQ_NORMAL     0
#define MP3_EQ_POP        1
#define MP3_EQ_ROCK       2
#define MP3_EQ_JAZZ       3
#define MP3_EQ_CLASSIC    4
#define MP3_EQ_BASS       5

#define MP3_SRC_SDCARD    1
#define MP3_SRC_BUILTIN   4

// Looping options, ALL, FOLDER, ONE and ONE_STOP are the 
// only ones that appear to do much interesting
//  ALL plays all the tracks in a repeating loop
//  FOLDER plays all the tracks in the same folder in a repeating loop
//  ONE plays the same track repeating
//  ONE_STOP does not loop, plays the track and stops
//  RAM seems to play one track and someties disables the ability to 
//  move to next/previous track, really weird.

#define MP3_LOOP_ALL      0
#define MP3_LOOP_FOLDER   1
#define MP3_LOOP_ONE      2
#define MP3_LOOP_RAM      3
#define MP3_LOOP_ONE_STOP 4
#define MP3_LOOP_NONE     4 

#define MP3_STATUS_STOPPED 0
#define MP3_STATUS_PLAYING 1
#define MP3_STATUS_PAUSED  2

// The response from a status query we get is  for some reason
// a bit... iffy, most of the time it is reliable, but sometimes
// instead of a playing (1) response, we get a paused (2) response
// even though it is playing.  Stopped responses seem reliable.
// So to work around this when getStatus() is called we actually
// request the status this many times and only if one of them is STOPPED
// or they are all in agreement that it is playing or paused then
// we return that status.  If some of them differ, we do another set 
// of tests etc...
#define MP3_STATUS_CHECKS_IN_AGREEMENT 4

#define MP3_DEBUG 0

class JQ6500_Serial 
{
  
  public: 

    /** Create JQ6500 object.
     * 
     * You can use any Serial libraries that implement a Stream object, for example:
     * 
     *     JQ6500_Serial mp3(Serial);
     * 
     * For a 5v Arduino:
     * -----------------
     *  * TX on JQ6500 connects to D8 on the Arduino
     *  * RX on JQ6500 connects to one end of a 1k resistor,
     *      other end of resistor connects to D9 on the Arduino
     * 
     * For a 3v3 Arduino:
     * -----------------
     *  * TX on JQ6500 connects to D8 on the Arduino
     *  * RX on JQ6500 connects to D9 on the Arduino
     * 
     * Of course, power and ground are also required, VCC on JQ6500 is 5v tolerant (but RX isn't totally, hence the resistor above).
     * 
     * And then you can use in your setup():
     * 
     *     mp3.begin(9600)
     *     mp3.reset();
     *
     * and all the other commands :-)
     */
    
    JQ6500_Serial(Stream *dev) : _serial(dev) {}
    JQ6500_Serial(Stream &dev) : _serial(&dev) {}
		
    /** Start playing the current file.
     */

    void play();
    
    /** Restart the current (possibly paused) track from the 
     *  beginning.
     *  
     *  Note that this is not an actual command the JQ6500 knows
     *  what we do is mute, advance to the next track, pause,
     *  unmute, and go back to the previous track (which will
     *  cause it to start playing.
     * 
     *  That said, it appears to work just fine.
     * 
     */
    
    void restart();
    
    /** Pause the current file.  To unpause, use play(),
     *  to unpause and go back to beginning of track use restart()
     */
    
    void pause();
    
    /** Play the next file.
     */
    
    void next();
    
    /** Play the previous file.
     */
    
    void prev();
    
    /** Play the next folder. 
     */
    
    void nextFolder();
    
    /** Play the previous folder. 
     */
    
    void prevFolder();
    
    /** Play a specific file based on it's (FAT table) index number.  Note that the index number
     *  has nothing to do with the file name (except if you uploaded/copied them to the media in
     *  order of file name).
     * 
     *  To sort your SD Card FAT table, search for a FAT sorting utility for your operating system 
     *  of choice.
     */
    
    void playFileByIndexNumber(unsigned int fileNumber);        
    
    /** Play a specific file in a specific folder based on the name of those folder and file.
     * 
     * Only applies to SD Card.
     * 
     * To use this function, folders must be named from 00 to 99, and the files in those folders
     * must be named from 000.mp3 to 999.mp3
     * 
     * So to play the file on the SD Card "/03/006.mp3" use mp3.playFileNumberInFolderNumber(3, 6);
     * 
     */
    
    void playFileNumberInFolderNumber(unsigned int folderNumber, unsigned int fileNumber);
    
    /** Increase the volume by 1 (volume ranges 0 to 30). */
    
    void volumeUp();
    
    /** Decrease the volume by 1 (volume ranges 0 to 30). */
    
    void volumeDn();
    
    /** Set the volume to a specific level (0 to 30). 
     * 
     * @param volumeFrom0To30 Level of volume to set from 0 to 30
     */
    
    void setVolume(byte volumeFrom0To30);
    
    /** Set the equalizer to one of 6 preset modes.
     * 
     * @param equalizerMode One of the following, 
     * 
     *  *  MP3_EQ_NORMAL
     *  *  MP3_EQ_POP        
     *  *  MP3_EQ_ROCK       
     *  *  MP3_EQ_JAZZ       
     *  *  MP3_EQ_CLASSIC    
     *  *  MP3_EQ_BASS       
     * 
     */
    
    void setEqualizer(byte equalizerMode); // EQ_NORMAL to EQ_BASS
    
    /** Set the looping mode.
     * 
     * @param loopMode One of the following,
     * 
     *  *  MP3_LOOP_ALL       - Loop through all files.
     *  *  MP3_LOOP_FOLDER    - Loop through all files in the same folder (SD Card only)
     *  *  MP3_LOOP_ONE       - Loop one file.
     *  *  MP3_LOOP_RAM       - Loop one file (uncertain how it is different to the previous!)
     *  *  MP3_LOOP_NONE      - No loop, just play one file and then stop. (aka MP3_LOOP_ONE_STOP)
     */
    
    void setLoopMode(byte loopMode);
    
    /** Set the source to read mp3 data from.
     * 
     *  @param source One of the following,
     * 
     *   * MP3_SRC_BUILTIN    - Files read from the on-board flash memory
     *   * MP3_SRC_SDCARD     - Files read from the SD Card (JQ6500-28P only)
     */
    
    void setSource(byte source);        // SRC_BUILTIN or SRC_SDCARD       
    
    /** Put the device to sleep.
     * 
     *  Not recommanded if you are using SD Card as for some reason
     *  it appears to cause the SD Card to not be recognised again
     *  until the device is totally powered off and on again :-/
     * 
     */
    
    void sleep();
    
    /** Reset the device (softly).
     *       
     * It may be necessary in practice to actually power-cycle the device
     * as sometimes it can get a bit confused, especially if changing
     * SD Cards on-the-fly which really doesn't work too well.
     * 
     * So if designing a PCB/circuit including JQ6500 modules it might be 
     * worth while to include such ability (ie, power the device through 
     * a MOSFET which you can turn on/off at will).
     * 
     */
    
    void reset();
    
    // Status querying commands
    /** Get the status from the device.
     * 
     * CAUTION!  This is somewhat unreliable for the following reasons...
     * 
     *  1. When playing from the on board memory (MP3_SRC_BUILTIN), STOPPED sems
     *     to never be returned, only PLAYING and PAUSED
     *  2. Sometimes PAUSED is returned when it is PLAYING, to try and catch this
     *     getStatus() actually queries the module several times to ensure that
     *     it is really sure about what it tells us.
     *
     * @return One of MP3_STATUS_PAUSED, MP3_STATUS_PLAYING and MP3_STATUS_STOPPED
     */
    
    byte getStatus();
    
    /** Get the current volume level.
     * 
     * @return Value between 0 and 30
     */
    
    byte getVolume();
    
    /** Get the equalizer mode.
     * 
     * @return One of the following, 
     * 
     *  *  MP3_EQ_NORMAL
     *  *  MP3_EQ_POP        
     *  *  MP3_EQ_ROCK       
     *  *  MP3_EQ_JAZZ       
     *  *  MP3_EQ_CLASSIC    
     *  *  MP3_EQ_BASS      
     */
    
    byte getEqualizer();
    
    /** Get loop mode.
     * 
     * @return One of the following,
     * 
     *  *  MP3_LOOP_ALL       - Loop through all files.
     *  *  MP3_LOOP_FOLDER    - Loop through all files in the same folder (SD Card only)
     *  *  MP3_LOOP_ONE       - Loop one file.
     *  *  MP3_LOOP_RAM       - Loop one file (uncertain how it is different to the previous!)
     *  *  MP3_LOOP_NONE      - No loop, just play one file and then stop. (aka MP3_LOOP_ONE_STOP)
     */
    
    byte getLoopMode();
    
    /** Count the number of files on the specified media.
     * 
     * @param source One of MP3_SRC_BUILTIN and MP3_SRC_SDCARD
     * @return Number of files present on that media.
     * 
     */
    
    unsigned int countFiles(byte source);    
    
    /** Count the number of folders on the specified media.
     * 
     *  Note that only SD Card can have folders.
     * 
     * @param source One of MP3_SRC_BUILTIN and MP3_SRC_SDCARD
     * @return Number of folders present on that media.
     */
    
    unsigned int countFolders(byte source);    
    
    /** For the currently playing (or paused, or file that would be played 
     *  next if stopped) file, return the file's (FAT table) index number.
     * 
     *  This number can be used with playFileByIndexNumber();
     * 
     *  @param source One of MP3_SRC_BUILTIN and MP3_SRC_SDCARD
     *  @return Number of file.
     */
    
    unsigned int currentFileIndexNumber(byte source);
 
    /** For the currently playing or paused file, return the 
     *  current position in seconds.
     * 
     * @return Number of seconds into the file currently played.
     * 
     */
    unsigned int currentFilePositionInSeconds();
    
    /** For the currently playing or paused file, return the 
     *  total length of the file in seconds.
     * 
     * @return Length of audio file in seconds.
     */
    
    unsigned int currentFileLengthInSeconds();
    
    /** Get the name of the "current" file on the SD Card.
     *  
     * The current file is the one that is playing, paused, or if stopped then
     * could be next to play or last played, uncertain.
     * 
     * It would be best to only consult this when playing or paused
     * and you know that the SD Card is the active source.
     * 
     * Unfortunately there is no way to query the device to find out
     * which media is the active source (at least not that I know of).
     * 
     */
    
    void currentFileName(char *buffer, unsigned int bufferLength);    
        
    
  protected:
    
    /** Send a command to the JQ6500 module, 
     * @param command        Byte value of to send as from the datasheet.
     * @param arg1           First (if any) argument byte
     * @param arg2           Second (if any) argument byte
     * @param responseBuffer Buffer to store a single line of response, if NULL, no response is read.
     * @param buffLength     Length of response buffer including NULL terminator.
     */
    
    void sendCommand(byte command, byte arg1, byte arg2, char *responseBuffer, unsigned int bufferLength);

    // Just some different versions of that for ease of use
    void sendCommand(byte command);    
    void sendCommand(byte command, byte arg1);    
    void sendCommand(byte command, byte arg1, byte arg2);
    
    /** Send a command to the JQ6500 module, and get a response.
     * 
     * For the query commands, the JQ6500 generally sends an integer response
     * (over the UART as 4 hexadecimal digits).
     * 
     * @param command        Byte value of to send as from the datasheet.
     * @return Response from module.
     */
    
    unsigned int sendCommandWithUnsignedIntResponse(byte command);

    // This seems not that useful since there only seems to be a version 1 anway :/
    unsigned int getVersion();
    
    int waitUntilAvailable(unsigned long maxWaitTime = 1000);
	
    Stream *_serial;
    
    static const uint8_t MP3_CMD_BEGIN = 0x7E;
    static const uint8_t MP3_CMD_END   = 0xEF;
    
    static const uint8_t MP3_CMD_PLAY = 0x0D;
    static const uint8_t MP3_CMD_PAUSE = 0x0E;
    static const uint8_t MP3_CMD_NEXT = 0x01;
    static const uint8_t MP3_CMD_PREV = 0x02;
    static const uint8_t MP3_CMD_PLAY_IDX = 0x03;
    
    static const uint8_t MP3_CMD_NEXT_FOLDER = 0x0F;
    static const uint8_t MP3_CMD_PREV_FOLDER = 0x0F; // Note the same as next, the data byte indicates direction
    
    static const uint8_t MP3_CMD_PLAY_FILE_FOLDER = 0x12;
    
    static const uint8_t MP3_CMD_VOL_UP = 0x04;
    static const uint8_t MP3_CMD_VOL_DN = 0x05;
    static const uint8_t MP3_CMD_VOL_SET = 0x06;
    
    static const uint8_t MP3_CMD_EQ_SET = 0x07;
    static const uint8_t MP3_CMD_LOOP_SET = 0x11;    
    static const uint8_t MP3_CMD_SOURCE_SET = 0x09;
    static const uint8_t MP3_CMD_SLEEP = 0x0A;    
    static const uint8_t MP3_CMD_RESET = 0x0C;
    
    static const uint8_t MP3_CMD_STATUS = 0x42;
    static const uint8_t MP3_CMD_VOL_GET = 0x43;
    static const uint8_t MP3_CMD_EQ_GET = 0x44;
    static const uint8_t MP3_CMD_LOOP_GET = 0x45;
    static const uint8_t MP3_CMD_VER_GET = 0x46;
    
    static const uint8_t MP3_CMD_COUNT_SD  = 0x47;
    static const uint8_t MP3_CMD_COUNT_MEM = 0x49;
    static const uint8_t MP3_CMD_COUNT_FOLDERS = 0x53;
    static const uint8_t MP3_CMD_CURRENT_FILE_IDX_SD = 0x4B;
    static const uint8_t MP3_CMD_CURRENT_FILE_IDX_MEM = 0x4D;
    
    static const uint8_t MP3_CMD_CURRENT_FILE_POS_SEC = 0x50;
    static const uint8_t MP3_CMD_CURRENT_FILE_LEN_SEC = 0x51;
    static const uint8_t MP3_CMD_CURRENT_FILE_NAME = 0x52;
    
};

#endif
