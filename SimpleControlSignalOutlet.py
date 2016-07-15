import __init__
import numpy as np
import time
from pylsl import StreamInfo, StreamOutlet, local_clock

# ==============================================
# ============ Constants =======================
# ==============================================
Fs = 32                 # Sampling rate. 32 control updates per second
channels = ["Control1"]
sine_freq = 0.5         # Frequency of control signal modulation
outlet_name = "SineWaveControl"
outlet_type = "Control"

# ==============================================
# =========== Create LSL Outlets ===============
# ==============================================

# Create a stream of fake 'raw' data with varying PAC values
control_info = StreamInfo(name=outlet_name, type=outlet_type,
                   channel_count=len(channels), nominal_srate=Fs,
                   channel_format='float32',
                   source_id='sinewavgen1234')
control_xml = control_info.desc()
chans = control_xml.append_child("channels")
for channame in channels:
    chn = chans.append_child("channel")
    chn.append_child_value("label", channame)
    chn.append_child_value("unit", "SDU")
    chn.append_child_value("type", "generated")
control_outlet = StreamOutlet(control_info)
print("Created outlet with name {} and type {}".format(outlet_name, outlet_type))

start_time = local_clock()           # Time of the 0th chunk
print("now sending data...")
try:
    last_time = start_time
    while True:
        if (local_clock() - last_time) < 1.0/Fs:
            time.sleep(0.001)
        last_time = local_clock()
        y = np.sin(2*np.pi*sine_freq*last_time)
        control_outlet.push_sample([y])

except KeyboardInterrupt:
    # No cleanup necessary?
    pass
