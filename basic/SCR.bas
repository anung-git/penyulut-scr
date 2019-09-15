$regfile = "m8def.dat"
$crystal = 12000000
Deflcdchar 0 , 12 , 18 , 18 , 12 , 32 , 32 , 32 , 32
Config Lcd = 16 * 2
Config Lcdpin = Pin , Db4 = Portd.6 , Db5 = Portd.7 , Db6 = Portb.0 , Db7 = Portb.1 , E = Portd.5 , Rs = Portd.4
Cursor Off Noblink
Config Int1 = Falling
Config Int0 = Falling
Config Timer1 = Timer , Prescale = 256
Config Timer0 = Timer , Prescale = 8
Config Portd.2 = Input
Config Portd.3 = Input
Config Portb.2 = Output
Config Adc = Single , Prescaler = Auto
Start Adc
Portd.3 = 1
Portd.2 = 1
Portb.2 = 0
On Int0 Sensor
On Int1 Cros
On Timer1 Count
On Timer0 Sulut
Scr Alias Portb.2
Const Konstan = 2812500
Dim Hasil As Single
Dim T As Long , Ts As Byte
Dim Sudut As Byte , Sudutref As Byte
Dim Flag_timer1 As Byte , Flag_int0 As Byte
Dim Potensio As Word , Calc As Single
Cls
Upperline
Lcd "PENYULUTAN SCR"
Lowerline
Lcd "STTNAS JOGJA"
Wait 3
Cls
Upperline

Enable Timer0
Enable Int0
Enable Int1
Enable Timer1
Enable Interrupts

Do
   Potensio = Getadc(4)
   Calc = Potensio * 180
   Calc = Calc / 1023
   Sudutref = Calc
   If Flag_timer1 = 1 Then
      T = T + &HFFFF
      If T > 140625 Then Hasil = 0
      Flag_timer1 = 0
   End If
   If Flag_int0 = 1 Then
     T = Ts + T
     Hasil = Konstan / T
     T = 0
     Flag_int0 = 0
   End If
   Upperline
   Lcd "Sudut=" ; Sudutref ; Chr(0) ; "  "
   Lowerline
   Lcd "Rpm="
   Lcd Fusing(hasil , "#.#") ; "         "
Loop

Count:
'Timer1 = 0
Flag_timer1 = 1
Return

Sensor:
Ts = Timer1
Timer1 = 0
Flag_int0 = 1
Return

Cros:
Sudut = 0
Scr = 0
Return

Sulut:
Timer0 = 178                                                ' 173
Incr Sudut
If Sudut > Sudutref Then Scr = 1
Return