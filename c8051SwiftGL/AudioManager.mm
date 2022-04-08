
#import "AudioManager.hpp"
#import <AVFoundation/AVFoundation.h>

void AudioManager::playCoinCollectedAudio() {
    NSURL *url = [[NSBundle mainBundle] URLForResource:@"coin_collected" withExtension:@"wav"];
    if (!url) { return; }
    
    SystemSoundID soundID;
    AudioServicesCreateSystemSoundID((__bridge  CFURLRef)
                                     url, &soundID);
    AudioServicesPlaySystemSound(soundID);
}
