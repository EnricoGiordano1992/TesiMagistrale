#!/usr/bin/python
import yaml, sys, getopt
import os

def main(argv):
    inputfile = ''
    output = ''
    proj_obj = {}
    try:
        opts, args = getopt.getopt(argv,"hi:o:",["ifile=","ofile="])
        if len(opts) < 2:
            print 'Usage: hl_to_systemc.py -i <inputfile> -o <outputfile>'
            sys.exit(2)

    except getopt.GetoptError:
        print 'Usage: hl_to_systemc.py -i <inputfile> -o <outputfile>'
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print 'Usage: hl_to_systemc.py -i <inputfile> -o <outputfile>'
            sys.exit()
        elif opt in ("-i", "--ifile"):
            inputfile = arg
        elif opt in ("-o", "--ofile"):
            output = arg
    print 'Input file:', inputfile
    print 'Output dir:', output

    main_file = open("template/main.cpp","r")
    main_file_text = main_file.read()
    modules_c_file = open("template/modules.cpp","r")
    modules_c_file_text = modules_c_file.read()
    modules_h_file = open("template/modules.h","r")
    modules_h_file_text = modules_h_file.read()
    top_c_file = open("template/Top.cpp","r")
    top_c_file_text = top_c_file.read()
    top_h_file = open("template/Top.h","r")
    top_h_file_text = top_h_file.read()
    specularity_file = open("template/specularity.h","r")
    specularity_file_text = specularity_file.read()

    with open(inputfile, 'r') as stream:
        try:
            proj_obj = yaml.load(stream)
        except yaml.YAMLError as exc:
            print(exc)


    print "generate strings..."

    all_top_fifo_name = "";
    all_modules_name = "";
    all_connector = "";
    add_modules = "";

    #create strings
    ##TOP.H
    ###___RC_TOP_PORT_N___
    print "generate Top strings..."
    top_h_file_text = top_h_file_text.replace("___RC_TOP_PORT_N___", str(proj_obj['port']['num']))

    ###___RC_TOP_PORT_TYPE___
    fifo_string = ""
    i = 0
    for value in proj_obj['port']['type']:
        i += 1
        fifo_string += "sc_fifo<"+value+">"
        if i < proj_obj['port']['num']:
            fifo_string += ","
    top_h_file_text = top_h_file_text.replace("___RC_TOP_PORT_TYPE___", fifo_string)

    ###___RC_TOP_CHANNELS_PORT_TYPE___
    channels_port_type_string = ""
    for obj in proj_obj['top-init']['channels']:
        channels_port_type_string += "\t\tsc_fifo<"+obj['type']+"> "+obj['name']+";\n"
        #save top fifo
        all_top_fifo_name += obj['name']+"(1) ,"
    top_h_file_text = top_h_file_text.replace("___RC_TOP_CHANNELS_PORT_TYPE___", channels_port_type_string)

    ###___RC_TOP_PORTAL_PORT_TYPE___
    portal_port_type_string = ""
    for obj in proj_obj['top-init']['portals']:
        portal_port_type_string += "\t\trc_portal<sc_fifo_"+obj['io-type']+"<"+obj['type']+"> "+obj['name']+";\n"
        #save portals name
        all_connector += obj['name']+", "
    all_connector = all_connector[:-2]
    top_h_file_text = top_h_file_text.replace("___RC_TOP_PORTAL_PORT_TYPE___", channels_port_type_string)

    ###___RC_RECONFIGURABLE_MODULES_DEF___
    reconfigurable_modules_def = "";
    for obj in proj_obj['reconfigurable-modules-init']['create']:
        reconfigurable_modules_def += "\t\t"+obj['type']+" "+obj['name']+";\n"
        #save modules name
        all_modules_name += obj['name']+"(\""+obj['type']+"\"), "
    top_h_file_text = top_h_file_text.replace("___RC_RECONFIGURABLE_MODULES_DEF___", reconfigurable_modules_def)

    ###___RC_TOP_VARIABLES___
    top_variables = ""
    for obj in proj_obj['top-init']['variables']:
        if "value" in obj:
            top_variables += "\t\t"+obj['type']+" "+obj['name']+" = "+str(obj['value'])+";\n"
        else:
            top_variables += "\t\t"+obj['type']+" "+obj['name']+";\n"
    top_h_file_text = top_h_file_text.replace("___RC_TOP_VARIABLES___", reconfigurable_modules_def)

    ###___RC_CTOR_INITIALIZER___
    rc_ctor_initializer = all_top_fifo_name + "ctrl(\"ctrl\"), "+all_modules_name+"stat_portmap("+all_connector+"), active_module(NULL)";
    top_h_file_text = top_h_file_text.replace("___RC_CTOR_INITIALIZER___", rc_ctor_initializer)

    ###___RC_BINDING_DYNAMIC_CONNECTOR___
    binding_dynamic_connector = "";
    for obj in proj_obj['reconfigurable-modules-init']['create']:
        binding_dynamic_connector += "\t\t\t\tconnector.bind_dynamic("+obj['name']+");\n"
    top_h_file_text = top_h_file_text.replace("___RC_BINDING_DYNAMIC_CONNECTOR___", binding_dynamic_connector)

    ###___RC_LOADING_LOADING_TIMES___
    loading_loading_times = "";
    for obj in proj_obj['reconfigurable-modules-init']['create']:
        loading_loading_times += "\t\t\t\t"+obj['name']+".rc_set_delay(RC_LOAD, sc_time("+str(obj['activation_delay'])+", "+obj['activation_delay_measure']+"));\n"
        # add module strings
        add_modules += obj['name']+"+"
    all_connector = all_connector[:-1]
    add_modules = add_modules[:-1]
    top_h_file_text = top_h_file_text.replace("___RC_LOADING_LOADING_TIMES___", loading_loading_times)

    ###___RC_ADD_MODULES___
    add_modules_string = "\t\t\t\tctrl.add("+add_modules+");"
    top_h_file_text = top_h_file_text.replace("___RC_ADD_MODULES___", add_modules_string)

    ###___RC_FIRST_MODULE___
    add_first_active_modules = "\t\t\t\tctrl.activate("+proj_obj['reconfigurable-modules-init']['first-module']+");\n"
    add_first_active_modules += "\t\t\t\tactive_module = &"+proj_obj['reconfigurable-modules-init']['first-module']+";\n"
    top_h_file_text = top_h_file_text.replace("___RC_FIRST_MODULE___", add_first_active_modules)


    ##TOP.C
    ###___TOP_PRE_CODE___
    top_pre_code=proj_obj['top-code']['pre-code']
    if proj_obj['top-code']['debug-activation']:
        top_pre_code += "\n#define DEBUG_ACTIVATION_TOP 1\n"
    top_c_file_text = top_c_file_text.replace("___TOP_PRE_CODE___", top_pre_code)

    ###___TOP_PRODUCER_VARIABLES___
    top_producer_variables = ""
    for obj in proj_obj['top-code']['producer']['variables']:
        if "value" in obj:
            top_producer_variables += "\t"+obj['type']+" "+obj['name']+" = "+obj['name']+";\n"
        else:
            top_producer_variables += "\t"+obj['type']+" "+obj['name']+";\n"
    top_c_file_text = top_c_file_text.replace("___TOP_PRODUCER_VARIABLES___", top_producer_variables)

    ###___TOP_PRODUCER_CODE___
    top_producer_code = proj_obj['top-code']['producer']['code']
    top_c_file_text = top_c_file_text.replace("___TOP_PRODUCER_CODE___", top_producer_code)

    ###___TOP_CONTROLLER_VARIABLES___
    top_controller_variables = ""
    for obj in proj_obj['top-code']['controller']['variables']:
        if "value" in obj:
            top_producer_variables += "\t"+obj['type']+" "+obj['name']+" = "+obj['name']+";\n"
        else:
            top_producer_variables += "\t"+obj['type']+" "+obj['name']+";\n"
    top_c_file_text = top_c_file_text.replace("___TOP_CONTROLLER_VARIABLES___", top_controller_variables)

    ###___TOP_CONTROLLER_CODE___
    top_controller_code = proj_obj['top-code']['controller']['code']
    top_c_file_text = top_c_file_text.replace("___TOP_CONTROLLER_CODE___", top_controller_code)

    ###___TOP_MONITOR_CODE___
    top_monitor_code = proj_obj['top-code']['monitor']['code']
    top_c_file_text = top_c_file_text.replace("___TOP_MONITOR_CODE___", top_monitor_code)

    ## MODULES.H
    print("generate modules.h strings...")
    ### ___RC_MODULES_PORT_N___
    modules_h_file_text = modules_h_file_text.replace("___RC_MODULES_PORT_N___", str(proj_obj['reconfigurable-modules-init']['port']['num']))

    ### ___RC_MODULES_PORT_TYPE___
    fifo_string = ""
    i = 0
    for value in proj_obj['reconfigurable-modules-init']['channels_in']:
        i += 1
        fifo_string += "\t\tsc_fifo<"+value['type']+">,\n"
    i = 0
    for value in proj_obj['reconfigurable-modules-init']['channels_out']:
        i += 1
        fifo_string += "\t\tsc_fifo<"+value['type']+">,\n"
    modules_h_file_text = modules_h_file_text.replace("___RC_MODULES_PORT_TYPE___", fifo_string)

    ### ___RC_RECONFIGURABLE_MODULES_CONSTRUCTORS___
    modules_constructors = ""
    fifo_string = ""
    fifo_names = ""
    pm = ""
    i = 0
    for value in proj_obj['reconfigurable-modules-init']['channels_in']:
        i += 1
        fifo_string += "\t\t\tsc_fifo<"+value['type']+"> "+value['name']+",\n"
        fifo_names = value['name']+", "

    i = 0
    for value in proj_obj['reconfigurable-modules-init']['channels_out']:
        i += 1
        fifo_string += "\t\t\tsc_fifo<"+value['type']+"> "+value['name']+",\n"
        fifo_names = value['name'] + ", "

    fifo_names = fifo_names[:-2]

    pm ="\tSTATES STATUS, NEXT_STATUS; \n"\
        "\tdynPortMap pm;\n"\
        "\tRC_RECONFIGURABLE_CTOR(Lying_configuration),\n"\
        "\t\tpm("+fifo_names+") // initialise the port map with the ports\n"\
        "\t\t{\n"\
        "\t\trc_add_portmap(pm);\n"\
        "\t\tRC_THREAD(proc);\n"\
        "\t\t}"

    fifo_string = fifo_string.replace(",", ";")
    fifo_string = fifo_string.replace("\t\t", "")

    for value in proj_obj['reconfigurable-modules-init']['create']:
        modules_constructors += "RC_RECONFIGURABLE_MODULE("+value['type']+")\n"
        modules_constructors += "{\n"
        modules_constructors += fifo_string
        modules_constructors += pm
        modules_constructors += "private:\n"\
                                "    void proc();\n"
        modules_constructors += "};\n"

    modules_h_file_text = modules_h_file_text.replace("___RC_RECONFIGURABLE_MODULES_CONSTRUCTORS___", modules_constructors)

    ## MODULES.C
    modules_c_file_text = "#include \"modules.h\"\n\n"+modules_c_file_text
    ### ___RC_RECONFIGURABLE_MODULES_COMMON_CODE___
    modules_c_file_text = modules_c_file_text.replace("___RC_RECONFIGURABLE_MODULES_COMMON_CODE___", proj_obj['modules-code']['common-code'])
    ### ___RC_RECONFIGURABLE_MODULES_IMPLEMENTATION_CODE___
    conf_code = ""
    for value in proj_obj['modules-code']['descriptions']:
        conf_code += "\n"
        conf_code += "void "+value['type']+"::proc()"\
                     "\n{"\
                     "\n\tSTATUS = S_INIT;\n\t" \
                     "\n\twhile (true) {"\
                     "\n\t\tNEXT_STATUS =  static_cast < STATES > (in1.read());"\
                     "\n\t\tRC_TRANSACTION {"\
                     "\n\t\t\tSTATUS = NEXT_STATUS;"\
                     "\n\t\t\tswitch(STATUS){"
        for state in value['fsm']:
            conf_code += "\n\t\t\t\tcase "+state['state']+":"\
                         "\n\t\t\t\t\t"+state['code']+""\
                         "\n\t\t\t\t\tbreak;\n"
        conf_code += "\n\t\t\t}"
        conf_code += "\n\t\t"+value['other-code']
        conf_code += "\n\t\t}"
        conf_code += "\n\t}"
        conf_code += "\n}"
    modules_c_file_text = modules_c_file_text.replace("___RC_RECONFIGURABLE_MODULES_IMPLEMENTATION_CODE___", conf_code)

    #save all
    print "generate files..."

    if not os.path.exists(output):
        os.makedirs(output)

    top_h_out_file = open(output+"/Top.h", "w")
    top_h_out_file.write(top_h_file_text)
    top_h_out_file.close()

    top_c_out_file = open(output+"/Top.cpp", "w")
    top_c_out_file.write(top_c_file_text)
    top_c_out_file.close()

    modules_h_out_file = open(output+"/modules.h", "w")
    modules_h_out_file.write(modules_h_file_text)
    modules_h_file.close()

    modules_c_out_file = open(output+"/modules.cpp", "w")
    modules_c_out_file.write(modules_c_file_text)
    modules_c_file.close()

    #END PROGRAM

    main_file.close()
    modules_c_file.close()
    modules_h_file.close()
    top_c_file.close()
    top_h_file.close()
    specularity_file.close()



if __name__ == "__main__":
    main(sys.argv[1:])
