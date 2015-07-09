#include <shared/et/typedefs.h>
#include <shared/et/osl.h>
#include <shared/et/bcmutils.h>
#include <shared/et/bcmendian.h>

uchar* bcm_ether_ntoa(char *ea, char *buf)
{
    sprintf(buf,"%02x:%02x:%02x:%02x:%02x:%02x",
        (uchar)ea[0]&0xff, (uchar)ea[1]&0xff, (uchar)ea[2]&0xff,
        (uchar)ea[3]&0xff, (uchar)ea[4]&0xff, (uchar)ea[5]&0xff);
    return ((uchar *) buf);
}

/* parse a xx:xx:xx:xx:xx:xx format ethernet address */
int bcm_ether_atoe(char *p, char *ea)
{
    int i = 0;

    for (;;) {
        ea[i++] = (char) bcm_strtoul(p, &p, 16);
        if (!*p++ || i == 6)
            break;
    }

    return (i == 6);
}

ulong bcm_strtoul(char *cp, char **endp, uint base)
{
	if (cp || endp || base) { }

	assert(FALSE);
	return 0;
}

int soc_hercules_mem_read_word(int unit, uint32 addr, void *word_data)
{
    if (unit || addr || word_data) { }

    assert(FALSE);
    return 0;
}

int soc_hercules_mem_write_word(int unit, uint32 addr, void *word_data)
{
    if (unit || addr || word_data) { }

    assert(FALSE);
    return 0;
}

int soc_hercules15_mmu_limits_config(int unit, int port, int num_ports,
                                     int num_cos, int lossless)
{
    if (unit || port || num_ports || num_cos || lossless) { }

    assert(FALSE);
    return 0;
}
