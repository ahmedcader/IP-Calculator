#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define B1 0
#define B2 1
#define B3 2
#define B4 3
#define CIDR 4

void SplitLiteralToIntArray(char address[], int address_block[], const char *delimeters);

int main(int argc, char** argv) {

    int address_block[4], netmask_block[4];

    char address[] = "128.42.0.0/21";

    SplitLiteralToIntArray(address, address_block, "./");

    char literal_netmask_bin[32];
    int bit_counter, net_bit_counter = 0;
    while (bit_counter != 33) {
        while (net_bit_counter != address_block[CIDR]) {
            literal_netmask_bin[bit_counter] = '1';
            net_bit_counter += 1;
            bit_counter += 1;
        }
        literal_netmask_bin[bit_counter] = '0';
        bit_counter += 1;
    }

    int i, block_counter, sqN_value, sqN = 0;
    bit_counter = 0;
    for (i = 0; i < 33; i++) {
        bit_counter += 1;
        if (literal_netmask_bin[i] == '1') {
            sqN_value += pow(2, 7 - sqN);
        }
        sqN += 1;
        if (bit_counter == 8) {
            netmask_block[block_counter] = sqN_value;
            bit_counter = 0;
            sqN = 0;
            sqN_value = 0;
            block_counter += 1;
        }
    }
    
    int wildcard_netmask_block[4];
    i = 0;
    for (i = 0; i < 4; i++) {
        wildcard_netmask_block[i] = ~netmask_block[i] & 0xFF;      
    }

    printf("Address:---------------------> %d.%d.%d.%d\n", address_block[B1], address_block[B2], address_block[B3], address_block[B4]);
    printf("Network Mask:----------------> %d.%d.%d.%d => %d [CIDR]\n", netmask_block[B1], netmask_block[B2], netmask_block[B3], netmask_block[B4], address_block[CIDR]);
    printf("Wildcard Mask:---------------> %d.%d.%d.%d\n", wildcard_netmask_block[B1], wildcard_netmask_block[B2], wildcard_netmask_block[B3], wildcard_netmask_block[B4]);
    printf("\n");
    printf("Network Address:-------------> %d.%d.%d.%d\n", address_block[B1] & netmask_block[B1], address_block[B2] & netmask_block[B2], address_block[B3] & netmask_block[B3], address_block[B4] & netmask_block[B4]);
    printf("Broadcast Address:-----------> %d.%d.%d.%d\n", wildcard_netmask_block[B1] | address_block[B1], wildcard_netmask_block[B2] | address_block[B2], wildcard_netmask_block[B3] | address_block[B3], wildcard_netmask_block[B4] | address_block[B4]);
    printf("\n");
    printf("Minimum Usable Address:------> %d.%d.%d.%d\n", address_block[B1] & netmask_block[B1], address_block[B2] & netmask_block[B2], address_block[B3] & netmask_block[B3], (address_block[B4] & netmask_block[B4]) + 1);
    printf("Maximum Usable Address:------> %d.%d.%d.%d\n", wildcard_netmask_block[B1] | address_block[B1], wildcard_netmask_block[B2] | address_block[B2], wildcard_netmask_block[B3] | address_block[B3], (wildcard_netmask_block[B4] | address_block[B4]) - 1);
    printf("Number of Hosts:-------------> %d\n", (int)pow(2, 32 - address_block[CIDR])-2);
}

void SplitLiteralToIntArray(char address[], int address_block[], const char *delimeters) {
    char * pch;
    pch = strtok(address, delimeters);
    int i = 0;
    while (pch != NULL) {
        address_block[i] = strtol(pch, NULL, 10);
        i += 1;
        pch = strtok(NULL, delimeters);
    }
}