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
    top_variables = "";
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
        #add module strings
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


    #save all
    print "generate files..."

    if not os.path.exists(output):
        os.makedirs(output)

    top_h_out_file = open(output+"/Top.h","w")
    top_h_out_file.write(top_h_file_text)
    top_h_out_file.close()

    top_c_out_file = open(output+"/Top.cpp","w")
    top_c_out_file.write(top_c_file_text)
    top_c_out_file.close()

    #END PROGRAM

    main_file.close()
    modules_c_file.close()
    modules_h_file.close()
    top_c_file.close()
    top_h_file.close()
    specularity_file.close()



if __name__ == "__main__":
    main(sys.argv[1:])
