#include <kinux/ide.h>
#include <kinux/io.h>
#include <kinux/printm.h>
#include <kinux/timer.h>

ide_channel channels[MAX_CHANNELS];
ide_device devices[MAX_IDE_DEVICES];
unsigned char ide_buf[2048] = {0};

unsigned char ide_read(unsigned char channel, unsigned char reg) {
  unsigned char result;

  if (reg > 0x07 && reg < 0x0C) {
    ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
  }

  if (reg < 0x08) {
    result = inb(channels[channel].base + reg - 0x00);
  } else if (reg < 0x0C) {
    result = inb(channels[channel].base + reg - 0x06);
  } else if (reg < 0x0E) {
    result = inb(channels[channel].ctrl + reg - 0x0A);
  } else if (reg < 0x16) {
    result = inb(channels[channel].bmide + reg - 0x0E);
  }

  if (reg > 0x07 && reg < 0x0C) {
    ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
  }

  return result;
}

void ide_write(unsigned char channel, unsigned char reg, unsigned char data) {
  if (reg > 0x07 && reg < 0x0C) {
    ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
  }

  if (reg < 0x08) {
    outb(channels[channel].base + reg - 0x00, data);
  } else if (reg < 0x0C) {
    outb(channels[channel].base + reg - 0x06, data);
  } else if (reg < 0x0E) {
    outb(channels[channel].ctrl + reg - 0x0A, data);
  } else if (reg < 0x16) {
    outb(channels[channel].bmide + reg - 0x0E, data);
  }

  if (reg > 0x07 && reg < 0x0C) {
    ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
  }
}

void insl(uint16_t reg, uint32_t *buffer, int quads) {
  for (int index = 0; index < quads; index++) {
    buffer[index] = inbl(reg);
  }
}

void outsl(uint16_t reg, uint32_t *buffer, int quads) {
  for (int index = 0; index < quads; index++) {
    outbl(reg, buffer[index]);
  }
}

void ide_read_buffer(uint8_t channel, uint8_t reg, uint32_t *buffer,
                     uint32_t quads) {
  if (reg > 0x07 && reg < 0x0C) {
    ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
  }

  asm("pushw %es");
  asm("movw %ds, %ax");
  asm("movw %ax, %es");

  if (reg < 0x08) {
    insl(channels[channel].base + reg - 0x00, buffer, quads);
  } else if (reg < 0x0C) {
    insl(channels[channel].base + reg - 0x06, buffer, quads);
  } else if (reg < 0x0E) {
    insl(channels[channel].ctrl + reg - 0x0A, buffer, quads);
  } else if (reg < 0x16) {
    insl(channels[channel].bmide + reg - 0x0E, buffer, quads);
  }

  asm("popw %es;");

  if (reg > 0x07 && reg < 0x0C) {
    ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
  }
}

unsigned char ide_polling(unsigned char channel, unsigned int advanced_check) {

  for (int i = 0; i < 4; i++) {
    ide_read(channel, ATA_REG_ALTSTATUS);
  }

  while (ide_read(channel, ATA_REG_STATUS) & ATA_SR_BSY)
    ;

  if (advanced_check) {
    unsigned char state = ide_read(channel, ATA_REG_STATUS);

    if (state & ATA_SR_ERR) {
      return 2;
    }

    if (state & ATA_SR_DF) {
      return 1;
    }

    if ((state & ATA_SR_DRQ) == 0) {
      return 3;
    }
  }

  return 0;
}

unsigned char ide_print_error(unsigned int drive, unsigned char err) {
  if (err == 0) {
    return err;
  }

  printm("IDE:");

  if (err == 1) {
    printm("- Device Fault\n");
    err = 19;
  } else if (err == 2) {
    unsigned char st = ide_read(devices[drive].channel, ATA_REG_ERROR);

    if (st & ATA_ER_AMNF) {
      printm("- No Address Mark Found\n");
      err = 7;
    }
    if (st & ATA_ER_TK0NF) {
      printm("- No Media or Media Error\n");
      err = 3;
    }
    if (st & ATA_ER_ABRT) {
      printm("- Command Aborted\n");
      err = 20;
    }
    if (st & ATA_ER_MCR) {
      printm("- No Media or Media Error\n");
      err = 3;
    }
    if (st & ATA_ER_IDNF) {
      printm("- ID mark not Found\n");
      err = 21;
    }
    if (st & ATA_ER_MC) {
      printm("- No Media or Media Error\n");
      err = 3;
    }
    if (st & ATA_ER_UNC) {
      printm("- Uncorrectable Data Error\n");
      err = 22;
    }
    if (st & ATA_ER_BBK) {
      printm("- Bad Sectors\n");
      err = 13;
    }
  } else if (err == 3) {
    printm("- Reads Nothing\n");
    err = 23;
  } else if (err == 4) {
    printm("- Write Protected\n");
    err = 8;
  }

  printm("- [%s %s] %s\n",
         (const char *[]){"Primary", "Secondary"}[devices[drive].channel],
         (const char *[]){"Master", "Slave"}[devices[drive].drive],
         devices[drive].model);

  return err;
}

void init_ide(unsigned int bar0, unsigned int bar1, unsigned int bar2,
              unsigned int bar3, unsigned int bar4) {

  printm("ide: initializing...\n");

  int i, j, k, count = 0;
  channels[ATA_PRIMARY].base = bar0;
  channels[ATA_PRIMARY].ctrl = bar1;
  channels[ATA_SECONDARY].base = bar2;
  channels[ATA_SECONDARY].ctrl = bar3;
  channels[ATA_PRIMARY].bmide = bar4;
  channels[ATA_SECONDARY].bmide = bar4;

  ide_write(ATA_PRIMARY, ATA_REG_CONTROL, 2);
  ide_write(ATA_SECONDARY, ATA_REG_CONTROL, 2);

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 2; j++) {
      uint8_t err = 0, type = IDE_ATA, status;
      devices[count].reserved = 0;

      ide_write(i, ATA_REG_HDDEVSEL, 0xA0 | (j << 4));
      // sleep(1);
      ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
      // sleep(1);

      if (ide_read(i, ATA_REG_STATUS) == 0) {
        continue;
      }

      while (1) {
        status = ide_read(i, ATA_REG_STATUS);
        if ((status & ATA_SR_ERR)) {
          err = 1;
          break;
        } //
        if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRQ)) {
          break;
        }
      }

      if (err != 0) {
        unsigned char cl = ide_read(i, ATA_REG_LBA1);
        unsigned char ch = ide_read(i, ATA_REG_LBA2);

        if (cl == 0x14 && ch == 0xEB) {
          type = IDE_ATAPI;
        } else if (cl == 0x69 && ch == 0x96) {
          type = IDE_ATAPI;
        } else {
          continue;
        }

        ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
        // sleep(1);
      }

      ide_read_buffer(i, ATA_REG_DATA, (unsigned int *)ide_buf, 128);
      devices[count].reserved = 1;
      devices[count].type = type;
      devices[count].channel = i;
      devices[count].drive = j;
      devices[count].sig =
          *((unsigned short *)(ide_buf + ATA_IDENT_DEVICETYPE));
      devices[count].features =
          *((unsigned short *)(ide_buf + ATA_IDENT_CAPABILITIES));
      devices[count].command_sets =
          *((unsigned int *)(ide_buf + ATA_IDENT_COMMANDSETS));

      if (devices[count].command_sets & (1 << 26)) {
        devices[count].size =
            *((unsigned int *)(ide_buf + ATA_IDENT_MAX_LBA_EXT));
      } else {
        devices[count].size = *((unsigned int *)(ide_buf + ATA_IDENT_MAX_LBA));
      }

      for (k = 0; k < 40; k += 2) {
        devices[count].model[k] = ide_buf[ATA_IDENT_MODEL + k + 1];
        devices[count].model[k + 1] = ide_buf[ATA_IDENT_MODEL + k];
      }
      devices[count].model[40] = '\0';

      for (k = 39; k >= 0; k--) {
        char ch = devices[count].model[k];
        if (ch == ' ') {
          devices[count].model[k] = '\0';
        } else {
          break;
        }
      }

      count++;
    }
  }

  for (i = 0; i < 4; i++)
    if (devices[i].reserved == 1) {
      printm("%d:-\n", i);
      printm("  model: %s\n", devices[i].model);
      printm("  type: %s\n", (const char *[]){"ATA", "ATAPI"}[devices[i].type]);
      printm("  drive: %u, channel: %u\n", devices[i].drive,
             devices[i].channel);
      printm("  base: 0x%x, control: 0x%x\n", channels[i].base,
             channels[i].ctrl);
      printm("  size: %u sectors, %u bytes\n", devices[i].size,
             devices[i].size * ATA_SECTOR_SIZE);
      printm("  signature: 0x%x, features: %d\n", devices[i].sig,
             devices[i].features);
    }
}
