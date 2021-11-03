####################################################################################################
#
# DebugInterface.py
#
# Tool for graphical display of status information and for controlling the DAQ v2 hardware via the 
# virtual serial interface (second USB-B port).
#
# pd, 2021-11-03
#
####################################################################################################
import PySimpleGUI as sg
import serial
import serial.tools.list_ports_windows

# global constants
SYSTEM_COLOR = '#f0f0f0'
LABEL_WIDTH = 25
INPUT_WIDTH = 20
RADIO_WIDTH = INPUT_WIDTH - 5
CHECKBOXLABEL_WIDTH = 20
CHECKBOX_WIDTH = 7
SLEEP_TIME = 0.25
GROUP_ID_INPUT_COUPLING_CH1_RADIO = 1           # do not change
GROUP_ID_INPUT_COUPLING_CH2_RADIO = 2           # do not change
GROUP_ID_INPUT_TYPE_CH1_RADIO = 3               # do not change
GROUP_ID_INPUT_TYPE_CH2_RADIO = 4               # do not change

# global variables
ser = serial.Serial()

# function executed when clicking on "System Reset" button
def button_system_reset():
    if ser.is_open:
        ser.write(bytes("SYSTEM_RESET;", encoding='utf8'))
        ser.flush()
        time.sleep(10*SLEEP_TIME)

# function executed when clicking on "Get Values" button
def button_get_values():
    get_parameter_input_coupling_ch1_radio("GET_INPUT_COUPL_CH1;")
    get_parameter_input_coupling_ch2_radio("GET_INPUT_COUPL_CH2;")
    get_parameter_input_type_ch1_radio("GET_INPUT_TYPE_CH1;")
    get_parameter_input_type_ch2_radio("GET_INPUT_TYPE_CH2;")
    get_parameter("GET_GAIN_VGA1;", 'key_vga1_dB')
    get_parameter("GET_GAIN_VGA2;", 'key_vga2_dB')
    get_parameter("GET_VOLT_DAC1;", 'key_dac1_V')
    get_parameter("GET_VOLT_DAC2;", 'key_dac2_V')

# function executed when clicking on "Set Values" button
def button_set_values():
    set_parameter_input_coupling_ch1_radio()
    set_parameter_input_coupling_ch2_radio()
    set_parameter_input_type_ch1_radio()
    set_parameter_input_type_ch2_radio()    
    set_parameter("SET_GAIN_VGA1 " + values['key_vga1_dB'] + ";")
    set_parameter("SET_GAIN_VGA2 " + values['key_vga2_dB'] + ";")

# function for getting certain parameter from the mainboard and writing the result to the GUI
def get_parameter(command, key):
    window.FindElement(key).update("0")
    window.Refresh()
    if ser.is_open:
        ser.write(bytes(command, encoding='utf8'))
        ser.flush()
        time.sleep(SLEEP_TIME)
        inbytes = ser.in_waiting
        if inbytes > 0:
            instring = str(ser.read(inbytes)).replace("b'", "").replace("\\n'", "")
            window.FindElement(key).update(instring)

# function for getting the input coupling of channel 1 and updating the according radio buttons
def get_parameter_input_coupling_ch1_radio(command):
    if ser.is_open:
        ser.write(bytes(command, encoding='utf8'))
        ser.flush()
        time.sleep(SLEEP_TIME)
        inbytes = ser.in_waiting
        if inbytes > 0:
            instring = str(ser.read(inbytes)).replace("b'", "").replace("\\n'", "")
            window.FindElement('key_input_coupling_ch1_ac').update(False)
            window.FindElement('key_input_coupling_ch1_dc').update(False)
            if (instring == "AC"):
                window.FindElement('key_input_coupling_ch1_ac').update(True)
            if (instring == "DC"):
                window.FindElement('key_input_coupling_ch1_dc').update(True)

# function for getting the input coupling of channel 2 and updating the according radio buttons
def get_parameter_input_coupling_ch2_radio(command):
    if ser.is_open:
        ser.write(bytes(command, encoding='utf8'))
        ser.flush()
        time.sleep(SLEEP_TIME)
        inbytes = ser.in_waiting
        if inbytes > 0:
            instring = str(ser.read(inbytes)).replace("b'", "").replace("\\n'", "")
            window.FindElement('key_input_coupling_ch2_ac').update(False)
            window.FindElement('key_input_coupling_ch2_dc').update(False)
            if (instring == "AC"):
                window.FindElement('key_input_coupling_ch2_ac').update(True)
            if (instring == "DC"):
                window.FindElement('key_input_coupling_ch2_dc').update(True)

# function for getting the input type of channel 1 and updating the according radio buttons
def get_parameter_input_type_ch1_radio(command):
    if ser.is_open:
        ser.write(bytes(command, encoding='utf8'))
        ser.flush()
        time.sleep(SLEEP_TIME)
        inbytes = ser.in_waiting
        if inbytes > 0:
            instring = str(ser.read(inbytes)).replace("b'", "").replace("\\n'", "")
            window.FindElement('key_input_type_ch1_s').update(False)
            window.FindElement('key_input_type_ch1_d').update(False)
            if (instring == "S"):
                window.FindElement('key_input_type_ch1_s').update(True)
            if (instring == "D"):
                window.FindElement('key_input_type_ch1_d').update(True)

# function for getting the input type of channel 2 and updating the according radio buttons
def get_parameter_input_type_ch2_radio(command):
    if ser.is_open:
        ser.write(bytes(command, encoding='utf8'))
        ser.flush()
        time.sleep(SLEEP_TIME)
        inbytes = ser.in_waiting
        if inbytes > 0:
            instring = str(ser.read(inbytes)).replace("b'", "").replace("\\n'", "")
            window.FindElement('key_input_type_ch2_s').update(False)
            window.FindElement('key_input_type_ch2_d').update(False)
            if (instring == "S"):
                window.FindElement('key_input_type_ch2_s').update(True)
            if (instring == "D"):
                window.FindElement('key_input_type_ch2_d').update(True)

# function for setting a parameter
def set_parameter(command):
    if ser.is_open:
        ser.write(bytes(command, encoding='utf8'))
        ser.flush()
        time.sleep(SLEEP_TIME)
        inbytes = ser.in_waiting
        ser.read(inbytes)

# function for setting the input coupling of channel 1
def set_parameter_input_coupling_ch1_radio():    
    time.sleep(SLEEP_TIME)
    if ser.is_open:
        if (bool(values['key_input_coupling_ch1_ac']) == True):
            ser.write(bytes("SET_INPUT_COUPL_CH1_AC;", encoding='utf8'))
        else:
            ser.write(bytes("SET_INPUT_COUPL_CH1_DC;", encoding='utf8'))
        ser.flush()
        time.sleep(SLEEP_TIME)
        inbytes = ser.in_waiting
        ser.read(inbytes)

# function for setting the input coupling of channel 2
def set_parameter_input_coupling_ch2_radio():    
    time.sleep(SLEEP_TIME)
    if ser.is_open:
        if (bool(values['key_input_coupling_ch2_ac']) == True):
            ser.write(bytes("SET_INPUT_COUPL_CH2_AC;", encoding='utf8'))
        else:
            ser.write(bytes("SET_INPUT_COUPL_CH2_DC;", encoding='utf8'))
        ser.flush()
        time.sleep(SLEEP_TIME)
        inbytes = ser.in_waiting
        ser.read(inbytes)

# function for setting the input type of channel 1
def set_parameter_input_type_ch1_radio():    
    time.sleep(SLEEP_TIME)
    if ser.is_open:
        if (bool(values['key_input_type_ch1_s']) == True):
            ser.write(bytes("SET_INPUT_TYPE_CH1_S;", encoding='utf8'))
        else:
            ser.write(bytes("SET_INPUT_TYPE_CH1_D;", encoding='utf8'))
        ser.flush()
        time.sleep(SLEEP_TIME)
        inbytes = ser.in_waiting
        ser.read(inbytes)

# function for setting the input type of channel 2
def set_parameter_input_type_ch2_radio():    
    time.sleep(SLEEP_TIME)
    if ser.is_open:
        if (bool(values['key_input_type_ch2_s']) == True):
            ser.write(bytes("SET_INPUT_TYPE_CH2_S;", encoding='utf8'))
        else:
            ser.write(bytes("SET_INPUT_TYPE_CH2_D;", encoding='utf8'))
        ser.flush()
        time.sleep(SLEEP_TIME)
        inbytes = ser.in_waiting
        ser.read(inbytes)

# set standard color theme
sg.theme('SystemDefault')

# get list of available serial ports
comports = []
ports = serial.tools.list_ports_windows.comports()
for i in range(0, len(ports)):
    comports.append(ports[i].device)

# layout of left column
column_left = [  
            [ sg.Text('') ],
            [ sg.Text('__________ Connection Settings __________') ],
            [ sg.Text('Port:', size=(LABEL_WIDTH,1)), sg.InputCombo(comports, default_value=comports[0], size=(INPUT_WIDTH-2,1), key='key_comports') ],
            [ sg.Text('Symbol rate [Bd]:', size=(LABEL_WIDTH,1)), sg.Input('115200', size=(INPUT_WIDTH,1), key='key_symbol_rate_Bd') ],
            [ sg.Text('Data bits:', size=(LABEL_WIDTH,1)), sg.Input('8', size=(INPUT_WIDTH,1), key='key_databits') ],
            [ sg.Text('Parity:', size=(LABEL_WIDTH,1)), sg.InputCombo(['None', 'Even', 'Odd'], default_value='None', size=(INPUT_WIDTH-2,1), key='key_parity') ],
            [ sg.Text('Stop bits:', size=(LABEL_WIDTH,1)), sg.Input('1', size=(INPUT_WIDTH,1), key='key_stopbits') ],
            [ sg.Text('Status:', size=(LABEL_WIDTH,1)), sg.Input('Disconnected', size=(INPUT_WIDTH,1), readonly=True, key='key_connection_status') ],
            [ sg.Text('') ],
            [ sg.Button('Exit', disabled=False, key='key_button_exit'), sg.Button('Connect', disabled=False, key='key_button_connect'), sg.Button('Disconnect', disabled=True, key='key_button_disconnect') ],
            [ sg.Button('System Reset', disabled=True, key='key_button_system_reset'), sg.Button('Get Values', disabled=True, key='key_button_get_values'), sg.Button('Set Values', disabled=True, key='key_button_set_values') ],
]

# layout of center column
column_center = [             
            [ sg.Text('__________ Input Coupling __________') ],            
            [ sg.Text('Channel 1:', size=(LABEL_WIDTH,1)), sg.Radio('AC', size=(RADIO_WIDTH,1), group_id=GROUP_ID_INPUT_COUPLING_CH1_RADIO, key='key_input_coupling_ch1_ac') ],
            [ sg.Text('', size=(LABEL_WIDTH,1)), sg.Radio('DC', size=(RADIO_WIDTH,1), group_id=GROUP_ID_INPUT_COUPLING_CH1_RADIO, key='key_input_coupling_ch1_dc') ],
            [ sg.Text('Channel 2:', size=(LABEL_WIDTH,1)), sg.Radio('AC', size=(RADIO_WIDTH,1), group_id=GROUP_ID_INPUT_COUPLING_CH2_RADIO, key='key_input_coupling_ch2_ac') ],
            [ sg.Text('', size=(LABEL_WIDTH,1)), sg.Radio('DC', size=(RADIO_WIDTH,1), group_id=GROUP_ID_INPUT_COUPLING_CH2_RADIO, key='key_input_coupling_ch2_dc') ],
            [ sg.Text('') ],
            [ sg.Text('__________ Input Type __________') ],            
            [ sg.Text('Channel 1:', size=(LABEL_WIDTH,1)), sg.Radio('Single Ended', size=(RADIO_WIDTH,1), group_id=GROUP_ID_INPUT_TYPE_CH1_RADIO, key='key_input_type_ch1_s') ],
            [ sg.Text('', size=(LABEL_WIDTH,1)), sg.Radio('Differential', size=(RADIO_WIDTH,1), group_id=GROUP_ID_INPUT_TYPE_CH1_RADIO, key='key_input_type_ch1_d') ],
            [ sg.Text('Channel 2:', size=(LABEL_WIDTH,1)), sg.Radio('Single Ended', size=(RADIO_WIDTH,1), group_id=GROUP_ID_INPUT_TYPE_CH2_RADIO, key='key_input_type_ch2_s') ],
            [ sg.Text('', size=(LABEL_WIDTH,1)), sg.Radio('Differential', size=(RADIO_WIDTH,1), group_id=GROUP_ID_INPUT_TYPE_CH2_RADIO, key='key_input_type_ch2_d') ]
]

# layout of right column
column_right = [   
            [ sg.Text('__________ Variable Gain Amplifiers __________') ],
            [ sg.Text('VGA 1 [dB]:', size=(LABEL_WIDTH,1)), sg.Input('', size=(INPUT_WIDTH,1), readonly=False, key='key_vga1_dB') ],
            [ sg.Text('VGA 2 [dB]:', size=(LABEL_WIDTH,1)), sg.Input('', size=(INPUT_WIDTH,1), readonly=False, key='key_vga2_dB') ],
            [ sg.Text('') ],
            [ sg.Text('DAC 1 [V]:', size=(LABEL_WIDTH,1)), sg.Input('', size=(INPUT_WIDTH,1), readonly=True, key='key_dac1_V') ],
            [ sg.Text('DAC 2 [V]:', size=(LABEL_WIDTH,1)), sg.Input('', size=(INPUT_WIDTH,1), readonly=True, key='key_dac2_V') ]
]

# overall layout
layout = [
    [
        sg.Column(column_left, element_justification='c'),
        sg.VSeperator(),
        sg.Column(column_center, element_justification='c'),
        sg.VSeperator(),
        sg.Column(column_right, element_justification='c')
    ]
]

# create new window
window = sg.Window("DAQ v2 | Debug Interface | pd (2021)", layout, finalize=True, margins=(25, 25), location=(100,150))

# run
while True:

    event, values = window.read()

    # connect to selected serial port
    if event == "key_button_connect":
        if not ser.is_open:
            ser.port = values['key_comports']
            ser.baudrate = float(values['key_symbol_rate_Bd'])
            ser.bytesize = float(values['key_databits'])
            if values['key_parity'] == 'None':
                ser.parity = serial.PARITY_NONE
            elif values['key_parity'] == 'Even':
                ser.parity = serial.PARITY_EVEN
            elif values['key_parity'] == 'Odd':
                ser.parity = serial.PARITY_ODD            
            ser.stopbits = float(values['key_stopbits'])            

            ser.open()
            if ser.is_open:
                window.FindElement('key_connection_status').update('Connected')
                window.FindElement('key_button_connect').update(disabled=True)
                window.FindElement('key_button_disconnect').update(disabled=False)
                window.FindElement('key_button_get_values').update(disabled=False)
                window.FindElement('key_button_set_values').update(disabled=False)
                window.FindElement('key_button_system_reset').update(disabled=False)
            else:
                window.FindElement('key_connection_status').update('Connection error')
                window.FindElement('key_button_connect').update(disabled=False)
                window.FindElement('key_button_disconnect').update(disabled=True)
                window.FindElement('key_button_get_values').update(disabled=True)
                window.FindElement('key_button_set_values').update(disabled=True)
                window.FindElement('key_button_system_reset').update(disabled=True)

    # system reset
    if event == "key_button_system_reset":
        window.FindElement('key_button_connect').update(disabled=True)
        window.FindElement('key_button_disconnect').update(disabled=True)
        window.FindElement('key_button_get_values').update(disabled=True)
        window.FindElement('key_button_set_values').update(disabled=True)
        window.FindElement('key_button_system_reset').update(disabled=True)
        button_system_reset()
        button_get_values()
        window.FindElement('key_button_connect').update(disabled=True)
        window.FindElement('key_button_disconnect').update(disabled=False)
        window.FindElement('key_button_get_values').update(disabled=False)
        window.FindElement('key_button_set_values').update(disabled=False)
        window.FindElement('key_button_system_reset').update(disabled=False)

    # get values
    if event == "key_button_get_values":
        window.FindElement('key_button_connect').update(disabled=True)
        window.FindElement('key_button_disconnect').update(disabled=True)
        window.FindElement('key_button_get_values').update(disabled=True)
        window.FindElement('key_button_set_values').update(disabled=True)
        window.FindElement('key_button_system_reset').update(disabled=True)
        button_get_values()
        window.FindElement('key_button_connect').update(disabled=True)
        window.FindElement('key_button_disconnect').update(disabled=False)
        window.FindElement('key_button_get_values').update(disabled=False)
        window.FindElement('key_button_set_values').update(disabled=False)
        window.FindElement('key_button_system_reset').update(disabled=False)

    # set values
    if event == "key_button_set_values":
        window.FindElement('key_button_connect').update(disabled=True)
        window.FindElement('key_button_disconnect').update(disabled=True)
        window.FindElement('key_button_get_values').update(disabled=True)
        window.FindElement('key_button_set_values').update(disabled=True)
        window.FindElement('key_button_system_reset').update(disabled=True)
        button_set_values()
        button_get_values()
        window.FindElement('key_button_connect').update(disabled=True)
        window.FindElement('key_button_disconnect').update(disabled=False)
        window.FindElement('key_button_get_values').update(disabled=False)
        window.FindElement('key_button_set_values').update(disabled=False)
        window.FindElement('key_button_system_reset').update(disabled=False)

    # disconnect serial port
    if event == "key_button_disconnect":
        if ser.is_open:
            ser.close()
        if not ser.is_open:
            window.FindElement('key_connection_status').update('Disonnected')
            window.FindElement('key_button_connect').update(disabled=False)
            window.FindElement('key_button_disconnect').update(disabled=True)
            window.FindElement('key_button_get_values').update(disabled=True)
            window.FindElement('key_button_set_values').update(disabled=True)
            window.FindElement('key_button_system_reset').update(disabled=True)

    # disconnect serial port and exit
    if event == "key_button_exit" or event == sg.WIN_CLOSED:
        if ser.is_open:
            ser.close()
        break

window.close()
