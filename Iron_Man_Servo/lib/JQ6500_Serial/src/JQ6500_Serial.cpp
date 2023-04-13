/** 
 * Arduino Library for JQ6500 MP3 Module
 * 
 * Copyright (C) 2014 James Sleeman, <http://sparks.gogo.co.nz/jq6500/index.html>
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

#include <Arduino.h>
#include "JQ6500_Serial.h"


void  JQ6500_Serial::play()
{
  this->sendCommand(MP3_CMD_PLAY);
}

void  JQ6500_Serial::restart()
{
  byte oldVolume = this->getVolume();  
  this->setVolume(0);
  this->next();
  this->pause();
  this->setVolume(oldVolume);
  this->prev();
}

void  JQ6500_Serial::pause()
{
  this->sendCommand(MP3_CMD_PAUSE);
}

void  JQ6500_Serial::next()
{
  this->sendCommand(MP3_CMD_NEXT);
}

void  JQ6500_Serial::prev()
{
  this->sendCommand(MP3_CMD_PREV);
}

void  JQ6500_Serial::playFileByIndexNumber(unsigned int fileNumber)
{  
  this->sendCommand(MP3_CMD_PLAY_IDX, (fileNumber>>8) & 0xFF, fileNumber & (byte)0xFF);
}

void  JQ6500_Serial::nextFolder()
{
  this->sendCommand(MP3_CMD_NEXT_FOLDER, 0x01);  
}

void  JQ6500_Serial::prevFolder()
{
  this->sendCommand(MP3_CMD_PREV_FOLDER, 0x00);
}

void  JQ6500_Serial::playFileNumberInFolderNumber(unsigned int folderNumber, unsigned int fileNumber)
{
  this->sendCommand(MP3_CMD_PLAY_FILE_FOLDER, folderNumber & 0xFF, fileNumber & 0xFF);
}

void  JQ6500_Serial::volumeUp()
{
  this->sendCommand(MP3_CMD_VOL_UP);
}

void  JQ6500_Serial::volumeDn()
{
  this->sendCommand(MP3_CMD_VOL_DN);
}

void  JQ6500_Serial::setVolume(byte volumeFrom0To30)
{
  this->sendCommand(MP3_CMD_VOL_SET, volumeFrom0To30);
}

void  JQ6500_Serial::setEqualizer(byte equalizerMode)
{
  this->sendCommand(MP3_CMD_EQ_SET, equalizerMode);
}

void  JQ6500_Serial::setLoopMode(byte loopMode)
{
  this->sendCommand(MP3_CMD_LOOP_SET, loopMode);
}

void  JQ6500_Serial::setSource(byte source)
{
  this->sendCommand(MP3_CMD_SOURCE_SET, source);
}

void  JQ6500_Serial::sleep()
{
  this->sendCommand(MP3_CMD_SLEEP);
}

void  JQ6500_Serial::reset()
{
  this->sendCommand(MP3_CMD_RESET);
  delay(500); // We need some time for the reset to happen
}


    byte  JQ6500_Serial::getStatus()    
    {
      byte statTotal = 0;
      byte stat       = 0;
      do
      {
        statTotal = 0;
        for(byte x = 0; x < MP3_STATUS_CHECKS_IN_AGREEMENT; x++)
        {
          stat = this->sendCommandWithUnsignedIntResponse(MP3_CMD_STATUS);      
          if(stat == 0) return 0; // STOP is fairly reliable
          statTotal += stat;
        }

      } while (statTotal != 1 * MP3_STATUS_CHECKS_IN_AGREEMENT && statTotal != 2 * MP3_STATUS_CHECKS_IN_AGREEMENT);
      
      return statTotal / MP3_STATUS_CHECKS_IN_AGREEMENT;      
    }
    
    byte  JQ6500_Serial::getVolume()    { return this->sendCommandWithUnsignedIntResponse(MP3_CMD_VOL_GET); }
    byte  JQ6500_Serial::getEqualizer() { return this->sendCommandWithUnsignedIntResponse(MP3_CMD_EQ_GET); }
    byte  JQ6500_Serial::getLoopMode()  { return this->sendCommandWithUnsignedIntResponse(MP3_CMD_LOOP_GET); }
    unsigned int  JQ6500_Serial::getVersion()   { return this->sendCommandWithUnsignedIntResponse(MP3_CMD_VER_GET); }
    
    unsigned int  JQ6500_Serial::countFiles(byte source)   
    {
      if(source == MP3_SRC_SDCARD)
      {
        return this->sendCommandWithUnsignedIntResponse(MP3_CMD_COUNT_SD); 
      }
      else if (source == MP3_SRC_BUILTIN)
      {
        return this->sendCommandWithUnsignedIntResponse(MP3_CMD_COUNT_MEM);
      }     
      
      return 0;
    }
    
    unsigned int  JQ6500_Serial::countFolders(byte source) 
    {
      if(source == MP3_SRC_SDCARD)
      {
        return this->sendCommandWithUnsignedIntResponse(MP3_CMD_COUNT_FOLDERS); 
      }
      
      return 0;
    }
    
    unsigned int  JQ6500_Serial::currentFileIndexNumber(byte source)
    {
      if(source == MP3_SRC_SDCARD)
      {
        return this->sendCommandWithUnsignedIntResponse(MP3_CMD_CURRENT_FILE_IDX_SD); 
      }
      else if (source == MP3_SRC_BUILTIN)
      {
        return this->sendCommandWithUnsignedIntResponse(MP3_CMD_CURRENT_FILE_IDX_MEM)+1; // CRAZY!
      }     
      
      return 0;
    }
    
    unsigned int  JQ6500_Serial::currentFilePositionInSeconds() { return this->sendCommandWithUnsignedIntResponse(MP3_CMD_CURRENT_FILE_POS_SEC); }
    unsigned int  JQ6500_Serial::currentFileLengthInSeconds()   { return this->sendCommandWithUnsignedIntResponse(MP3_CMD_CURRENT_FILE_LEN_SEC); }
    
    void          JQ6500_Serial::currentFileName(char *buffer, unsigned int bufferLength) 
    {
      this->sendCommand(MP3_CMD_CURRENT_FILE_NAME, 0, 0, buffer, bufferLength);
    }
    
    // Used for the status commands, they mostly return an 8 to 16 bit integer 
    // and take no arguments
    unsigned int JQ6500_Serial::sendCommandWithUnsignedIntResponse(byte command)
    {      
      char buffer[5];
      this->sendCommand(command, 0, 0, buffer, sizeof(buffer));
      return (unsigned int) strtoul(buffer, NULL, 16);
    }
    
    void  JQ6500_Serial::sendCommand(byte command)
    {
      this->sendCommand(command, 0, 0, 0, 0);
    }
    
    void  JQ6500_Serial::sendCommand(byte command, byte arg1)
    {
       this->sendCommand(command, arg1, 0, 0, 0);
    }
    
    void  JQ6500_Serial::sendCommand(byte command, byte arg1, byte arg2)
    {
       this->sendCommand(command, arg1, arg2, 0, 0);
    }    
    
    void  JQ6500_Serial::sendCommand(byte command, byte arg1, byte arg2, char *responseBuffer, unsigned int bufferLength)
    {
      
      
      // Command structure
      // [7E][number bytes following including command and terminator][command byte][?arg1][?arg2][EF]
      
      // Most commands do not have arguments
      byte args = 0;
      
      // These ones do
      switch(command)
      {        
        case MP3_CMD_PLAY_IDX: args = 2; break;
        case MP3_CMD_VOL_SET: args = 1; break;
        case MP3_CMD_EQ_SET: args = 1; break;        
        case MP3_CMD_SOURCE_SET: args = 1; break;
        case MP3_CMD_PREV_FOLDER: args = 1; break; // Also MP3_CMD_NEXT_FOLDER
        case MP3_CMD_LOOP_SET: args = 1; break;
        case MP3_CMD_PLAY_FILE_FOLDER: args = 2; break;
      }
      
#if MP3_DEBUG
      char buf[4];       
      Serial.println();
      Serial.print(MP3_CMD_BEGIN, HEX); Serial.print(" ");
      itoa(2+args, buf, 16); Serial.print(buf); Serial.print(" "); memset(buf, 0, sizeof(buf));
      itoa(command, buf, 16); Serial.print(buf); Serial.print(" "); memset(buf, 0, sizeof(buf));
      if(args>=1) itoa(arg1, buf, 16); Serial.print(buf); Serial.print(" "); memset(buf, 0, sizeof(buf));
      if(args>=2) itoa(arg2, buf, 16); Serial.print(buf); Serial.print(" "); memset(buf, 0, sizeof(buf));
      Serial.print(MP3_CMD_END, HEX);      
#endif
      
      // The device appears to send some sort of status information (namely "STOP" when it stops playing)
      // just discard this right before we send the command
      while(this->waitUntilAvailable(10)) _serial->read();
      
      _serial->write((byte)MP3_CMD_BEGIN);
      _serial->write(2+args);
      _serial->write(command);
      if(args>=1) _serial->write(arg1);
      if(args==2) _serial->write(arg2);
      _serial->write((byte)MP3_CMD_END);
      
      
      unsigned int i = 0;
      char         j = 0;
      if(responseBuffer && bufferLength) 
      {
        memset(responseBuffer, 0, bufferLength);
      }
      
      // Allow some time for the device to process what we did and 
      // respond, up to 1 second, but typically only a few ms.
      this->waitUntilAvailable(1000);

      
#if MP3_DEBUG
      Serial.print(" ==> [");
#endif
      
      while(this->waitUntilAvailable(150))
      {
        j = (char)_serial->read();
        
#if MP3_DEBUG
        Serial.print(j);
#endif
        if(responseBuffer && (i<(bufferLength-1)))
        {
           responseBuffer[i++] = j;
        }
      }
      
#if MP3_DEBUG      
      Serial.print("]");
      Serial.println();
#endif
      
    }
    

// Waits until data becomes available, or a timeout occurs
int JQ6500_Serial::waitUntilAvailable(unsigned long maxWaitTime)
{
  unsigned long startTime;
  int c = 0;
  startTime = millis();
  do {
    c = _serial->available();
    if (c) break;
  } while(millis() - startTime < maxWaitTime);
  
  return c;
}
