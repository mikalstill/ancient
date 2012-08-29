/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher 
 * Copyright: GPL V2
 *
 * This file can be used to decide which functionallity of the
 * TCP/IP stack shall be available.
 *
 *********************************************/
//@{
#ifndef IP_CONFIG_H
#define IP_CONFIG_H

// functions to decode cgi-form data:
#undef FROMDECODE_websrv_help

// function to encode a URL (mostly needed for a web client)
#undef URLENCODE_websrv_help

#endif /* IP_CONFIG_H */
//@}
