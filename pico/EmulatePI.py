from MotorDriverDrv8825 import *

class EmulatePI:
  def __init__(self, msa, msb):
    self.lut_cnt = 0

    # aFFFF is sma and b is smb, rename machines later
    self.emulate_lut = [ "aFFFFbFFFF",
                         "a8000b8000", # Stop
                         "aFFF0bFFF0", # Speed up
                         "a00FFb00FF", # Slow down
                         "aFFFFbF00F", # Got different speeds
                         "a7FFFb7FFF"] # Change direction

  def pps_to_delay(self, val):
    return(int(((1/val)*1000000)/3) - 4)

  def get_dir(self, i): # integer
    # Upper bit is direction
    return(0x8000 & i)

  def convert_to_int(self, s):
    return(int(s, 16))

  def convert_to_pps(self, i):
    # HJA: We can range check here since a range of 18 to 6400 is plenty
    # All lower bits are speed in pps
    return(0x7fff & i)

  def split_lut(self, luts):
    sma_lut = ""
    smb_lut = ""
    lut_buf = ""

    if (len(luts) < 10):
      return(sma_lut, smb_lut)

    i = luts.index("a") # Find ms a string start
    sma_lut = luts[i+1:i+5]

    i = luts.index("b") # Find ms a string start
    smb_lut = luts[i+1:i+5]

    return(sma_lut, smb_lut)

  def emulate(self, msa, msb):
    lut_cnt=0
    if (++lut_cnt > len(emulate_lut)):
      lut_cnt = 0

    (sma_s, smb_s) = split_lut(emulate_lut[lut_cnt])

    sma_i = convert_to_int(sma_s)
    # set_motor_a_dir(get_dir(sma_i))

    smb_i = convert_to_int(smb_s)
    # set_motor_b_dir(get_dir(smb_i))

    del_a = pps_to_delay(convert_to_pps(sma_i))
    del_b = pps_to_delay(convert_to_pps(smb_i))

    print(sma_s, "del_a =", del_a, " sma_i =", sma_i)
    sma.put(del_a)
    print(smb_s, "del_b =", del_b, " smb_i =", smb_i)
    smb.put(del_b)

    sleep(5)
