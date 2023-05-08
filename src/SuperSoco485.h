/**
 *
 */

#ifndef SUPER_SOCO_485_H
#define SUPER_SOCO_485_H

typedef void (*ParsedTelegramReceived)(void *sender, void *telegram);

class SuperSoco485
{
public:
    SuperSoco485();

    void begin();

    void setTelegramReceived(ParsedTelegramReceived callback);
    void handleData();

private:
    ParsedTelegramReceived _callback;
};

#endif