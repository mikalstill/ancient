/*
 * Decode and print the name of the connector the CPU plugs into.
 */

#include <stdio.h>
#include "x86info.h"

void decode_connector(unsigned int type)
{
	/* generic sockets */
	char conn_socket_3[] = "Socket 3 (PGA socket)";
	char conn_socket_4[] = "Socket 4 (273 pin PGA)";
	char conn_socket_5[] = "Socket 5 (296 pin PGA socket)";
	char conn_socket_7[] = "Socket 7 (321 pin PGA socket)";
	char conn_socket_370[] = "Socket 370  (370 Pin PGA)";
	char conn_socket_370FCPGA[] = "Socket 370 (FC-PGA) or (PPGA)";
	/* AMD specific sockets */
	char conn_socket_5_7[] = "Socket 5/7 (296 Pin PGA)";
	char conn_super_socket_7[] = "Super Socket 7 (321 Pin PGA)";
	char conn_slot_a[] = "Slot A (242 Contact Cartridge)";
	char conn_socket_a[] = "Socket A (462 Pin PGA)";
	char conn_socketA_or_slotA[] = "Socket A or Slot A";
	char conn_socket_754[] = "Socket 754";
	/* Intel specific sockets */
	char conn_socket_57b[] = "Socket 5/7 (320 Pin PGA)";
	char conn_mobile_7[] = "Mobile Module (320 Lead TCP)";
	char conn_socket_8[] = "Socket 8 (387 pin Dual Cavity PGA)";
	char conn_slot_1[] = "Slot 1 (242 Contact Cartridge)";
	char conn_slot_2[] = "Slot 2 (SEC Cartridge)";
	char conn_socket_423[] = "Socket423 (PGA423 Socket)";
	char conn_socket_478[] = "Socket478 (PGA478 Socket)";
	char conn_MMC[] = "Mobile Module Connector (BGA)";
	char conn_MMC2[] = "Mobile Module Connector (MMC-2)";
	/* Transmeta specific sockets */
	char conn_BGA474[] = "474 Pin BGA";
	/* natsemi specific (Geode) */
	char conn_BGA[] = "BGA";

	if (type == CONN_UNKNOWN)
		return;

	output(msg_format, "Connector type: ");
	switch (type) {
	case CONN_SOCKET_3:
		output(msg_cpuconn, conn_socket_3);
		break;

	case CONN_SOCKET_4:
		output(msg_cpuconn, conn_socket_4);
		break;

	case CONN_SOCKET_5:
		output(msg_cpuconn, conn_socket_5);
		break;

	case CONN_SOCKET_7:
		output(msg_cpuconn, conn_socket_7);
		break;

	case CONN_SOCKET_370:
		output(msg_cpuconn, conn_socket_370);
		break;

	case CONN_SOCKET_370_FCPGA:
		output(msg_cpuconn, conn_socket_370FCPGA);
		break;

	case CONN_SOCKET_5_7:
		output(msg_cpuconn, conn_socket_5_7);
		break;

	case CONN_SUPER_SOCKET_7:
		output(msg_cpuconn, conn_super_socket_7);
		break;

	case CONN_SLOT_A:
		output(msg_cpuconn, conn_slot_a);
		break;

	case CONN_SOCKET_A:
		output(msg_cpuconn, conn_socket_a);
		break;

	case CONN_SOCKET_A_OR_SLOT_A:
		output(msg_cpuconn, conn_socketA_or_slotA);
		break;

	case CONN_SOCKET_754:
		output(msg_cpuconn, conn_socket_754);
		break;

	case CONN_SOCKET_57B:
		output(msg_cpuconn, conn_socket_57b);
		break;

	case CONN_MOBILE_7:
		output(msg_cpuconn, conn_mobile_7);
		break;

	case CONN_SOCKET_8:
		output(msg_cpuconn, conn_socket_8);
		break;

	case CONN_SLOT_1:
		output(msg_cpuconn, conn_slot_1);
		break;

	case CONN_SOCKET_423:
		output(msg_cpuconn, conn_socket_423);
		break;

	case CONN_SOCKET_478:
		output(msg_cpuconn, conn_socket_478);
		break;

	case CONN_MMC:
		output(msg_cpuconn, conn_MMC);
		break;

	case CONN_MMC2:
		output(msg_cpuconn, conn_MMC2);
		break;

	case CONN_SLOT_2:
		output(msg_cpuconn, conn_slot_2);
		break;

	case CONN_BGA474:
		output(msg_cpuconn, conn_BGA474);
		break;

	case CONN_BGA:
		output(msg_cpuconn, conn_BGA);
		break;

	default:
		output(msg_cpuconn, "unknown");
		break;
	}

	output(msg_format, "\n\n");

}
