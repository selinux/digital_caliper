/*
 * =====================================================================================
 *
 *       Filename:  read_caliper.ino
 *
 *    Description:  Read digital caliper and send result through serial line 
 *
 *        Version:  1.0
 *        Created:  13. 09. 14 12:04:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sebastien Chassot (sinux), sebastien@sinux.net
 *        Company:  Post Tenebras Lab, the geneva's hackerspace
 *
 * =====================================================================================
 */


#define DATA     2
#define CLOCK    3


void setup() {

    /* Init serial */
    Serial.begin(115200);
    Serial.println("Init serial...");

    // initialize the digital pin as an input.
    pinMode(DATA, INPUT);
    pinMode(CLOCK, INPUT);

}


void loop() {

    byte pos;            // the byte position
    int mesure;          // integer result

    int time = millis(); // used to test the frame's lenght

    /* start main sequence (canceled later if wasn't the begining) */
    if( digitalRead(CLOCK) == LOW ){

        pos = 0;
        mesure = 0;

        /* skip first bit (unused) */
        while( digitalRead(CLOCK) == LOW ){
        }

        /* parse byte 1 to 20 */
        for( pos = 1; pos < 21; pos++ ){

            /* wait a falling edge */
            while( digitalRead(CLOCK) == HIGH ){
            }

            /* set bit if HIGH */
            if ( digitalRead(DATA) == HIGH )
                mesure |= ( 1 << pos );

            /* wait */
            while( digitalRead(CLOCK) == LOW ){
            }
        }

        /* 21th falling edge */
        while( digitalRead(CLOCK) == HIGH ){
        }

        /* sign HIGH mean neg (bit 21)*/
        if( digitalRead(DATA) == HIGH )
            mesure = ~mesure + 1;   // two complement (32 bits now)


        /* sequence too long for a frame */
        time = millis() - time;
        if( time < 30 )
            delay(30);      // skip to idle position (200ms beetwen frame)
        
        /* first frame could be corrupt */
        Serial.println((float)mesure/200);

    }

}
