class SignalResponse():
    def __init__(self,timeStamps,auxiliary,amplifier,adc,ttlIn,ttlOut, unsorted):
        self.timeStamps=timeStamps
        self.auxiliary=auxiliary
        self.amplifier=amplifier
        self.adc=adc
        self.ttlIn=ttlIn
        self.ttlOut=ttlOut
        self.unsorted=unsorted
