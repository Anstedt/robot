class EmulatePI:
  def __init__(self, msa, msb):
    self.lut_cnt = 0

  def pps_to_delay(self, val):
    if (val <= 0):
      return(0)
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

  # aFFFF is sma and b is smb, rename machines later
  emulate_lut = [ "x8012y8012", # 18
                  "x8064y8064", # 100 Stop, HJA hack to handle design does not handle 0 speed yet
                  "x80C8y80C8", # 200 Speed up
                  "x8190y8190", # 400 Slow down
                  "x83E8y83E8", # 1000 Got different speeds
                  "x87D0y87D0", # 2000
                  "x8000y8000"] # Stop

  def get_lut_len(self):
    print("lut_len= ", len(emulate_lut))
    return(len(emulate_lut))
  
  def split_lut(self, luts):
    sma_lut = ""
    smb_lut = ""
    lut_buf = ""

    if (len(luts) < 10):
      print("Error command ", luts, " is less than 10")
      return(sma_lut, smb_lut)

    i = luts.index("x") # Find ms a string start
    sma_lut = luts[i+1:i+5]

    i = luts.index("y") # Find ms a string start
    smb_lut = luts[i+1:i+5]

    return(sma_lut, smb_lut)

  def emulate(self, msa, msb, lut_cnt):
    (sma_s, smb_s) = split_lut(emulate_lut[lut_cnt])

    sma_i = convert_to_int(sma_s)
    # set_motor_a_dir(get_dir(sma_i))

    smb_i = convert_to_int(smb_s)
    # set_motor_b_dir(get_dir(smb_i))

    print(lut_cnt, sma_s, " sma_i =", sma_i, " pps =", convert_to_pps(sma_i))
    print(lut_cnt, smb_s, " smb_i =", smb_i, " pps =", convert_to_pps(sma_i))

    del_a = pps_to_delay(convert_to_pps(sma_i))
    del_b = pps_to_delay(convert_to_pps(smb_i))

    print(sma_s, "del_a =", del_a, " sma_i =", sma_i)
    sma.put(del_a)

    print(smb_s, "del_b =", del_b, " smb_i =", smb_i)
    smb.put(del_b)
