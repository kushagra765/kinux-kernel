#include <kinux/console.h>
#include <kinux/ide.h>
#include <kinux/io.h>
#include <kinux/printm.h>
#include <kinux/timer.h>
#include <lib/string.h>

ide_channel channels[MAX_CHANNELS];
ide_device devices[MAX_IDE_DEVICES];
unsigned char ide_buf[2048] = {0};
volatile unsigned static int ide_irq_invoked = 0;

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

  __asm__ volatile("pushw %es");
  __asm__ volatile("movw %ds, %ax");
  __asm__ volatile("movw %ax, %es");

  if (reg < 0x08) {
    insl(channels[channel].base + reg - 0x00, buffer, quads);
  } else if (reg < 0x0C) {
    insl(channels[channel].base + reg - 0x06, buffer, quads);
  } else if (reg < 0x0E) {
    insl(channels[channel].ctrl + reg - 0x0A, buffer, quads);
  } else if (reg < 0x16) {
    insl(channels[channel].bmide + reg - 0x0E, buffer, quads);
  }

  __asm__ volatile("popw %es;");

  if (reg > 0x07 && reg < 0x0C) {
    ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
  }
}

void ide_write_buffer(uint8_t channel, uint8_t reg, uint32_t *buffer,
                      uint32_t quads) {
  if (reg > 0x07 && reg < 0x0C) {
    ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
  }

  __asm__ volatile("pushw %es");
  __asm__ volatile("movw %ds, %ax");
  __asm__ volatile("movw %ax, %es");

  if (reg < 0x08) {
    outsl(channels[channel].base + reg - 0x00, buffer, quads);
  } else if (reg < 0x0C) {
    outsl(channels[channel].base + reg - 0x06, buffer, quads);
  } else if (reg < 0x0E) {
    outsl(channels[channel].ctrl + reg - 0x0A, buffer, quads);
  } else if (reg < 0x16) {
    outsl(channels[channel].bmide + reg - 0x0E, buffer, quads);
  }

  __asm__ volatile("popw %es;");

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
  } else if (err == -3) {
    printm("- Reads Nothing\n");
    err = 23;
  } else if (err == -4) {
    printm("- Write Protected\n");
    err = 8;
  }

  printm("- [%s %s] %s\n",
         (const char *[]){"Primary", "Secondary"}[devices[drive].channel],
         (const char *[]){"Master", "Slave"}[devices[drive].drive],
         devices[drive].model);

  return err;
}

int ide_ata_access(unsigned char direction, unsigned char drive,
                   unsigned int lba, unsigned char numsects, uint32_t buf) {
  unsigned char lba_mode, dma, cmd;
  unsigned char lba_io[6];
  unsigned int channel = devices[drive].channel;
  unsigned int slavebit = devices[drive].drive;
  unsigned int bus = channels[channel].base;
  unsigned int words = 256;
  unsigned short cyl, i;
  unsigned char head, sect, err;

  ide_write(channel, ATA_REG_CONTROL,
            channels[channel].nIEN = (ide_irq_invoked = 0x0) + 0x02);

  if (lba >= 0x10000000) {
    lba_mode = 2;
    lba_io[0] = (lba & 0x000000FF) >> 0;
    lba_io[1] = (lba & 0x0000FF00) >> 8;
    lba_io[2] = (lba & 0x00FF0000) >> 16;
    lba_io[3] = (lba & 0xFF000000) >> 24;
    lba_io[4] = 0;
    lba_io[5] = 0;
    head = 0;
  } else if (devices[drive].features & 0x200) {
    lba_mode = 1;
    lba_io[0] = (lba & 0x00000FF) >> 0;
    lba_io[1] = (lba & 0x000FF00) >> 8;
    lba_io[2] = (lba & 0x0FF0000) >> 16;
    lba_io[3] = 0;
    lba_io[4] = 0;
    lba_io[5] = 0;
    head = (lba & 0xF000000) >> 24;
  } else {
    lba_mode = 0;
    sect = (lba % 63) + 1;
    cyl = (lba + 1 - sect) / (16 * 63);
    lba_io[0] = sect;
    lba_io[1] = (cyl >> 0) & 0xFF;
    lba_io[2] = (cyl >> 8) & 0xFF;
    lba_io[3] = 0;
    lba_io[4] = 0;
    lba_io[5] = 0;
    head = (lba + 1 - sect) % (16 * 63) / (63);
  }

  dma = 0;

  while (ide_read(channel, ATA_REG_STATUS) & ATA_SR_BSY) {
  }

  if (lba_mode == 0) {
    ide_write(channel, ATA_REG_HDDEVSEL, 0xA0 | (slavebit << 4) | head);
  } else {
    ide_write(channel, ATA_REG_HDDEVSEL, 0xE0 | (slavebit << 4) | head);
  }

  if (lba_mode == 2) {
    ide_write(channel, ATA_REG_SECCOUNT1, 0);
    ide_write(channel, ATA_REG_LBA3, lba_io[3]);
    ide_write(channel, ATA_REG_LBA4, lba_io[4]);
    ide_write(channel, ATA_REG_LBA5, lba_io[5]);
  }

  ide_write(channel, ATA_REG_SECCOUNT0, numsects);
  ide_write(channel, ATA_REG_LBA0, lba_io[0]);
  ide_write(channel, ATA_REG_LBA1, lba_io[1]);
  ide_write(channel, ATA_REG_LBA2, lba_io[2]);

  if (lba_mode == 0 && dma == 0 && direction == 0) {
    cmd = ATA_CMD_READ_PIO;
  }
  if (lba_mode == 1 && dma == 0 && direction == 0) {
    cmd = ATA_CMD_READ_PIO;
  }
  if (lba_mode == 2 && dma == 0 && direction == 0) {
    cmd = ATA_CMD_READ_PIO_EXT;
  }
  if (lba_mode == 0 && dma == 1 && direction == 0) {
    cmd = ATA_CMD_READ_DMA;
  }
  if (lba_mode == 1 && dma == 1 && direction == 0) {
    cmd = ATA_CMD_READ_DMA;
  }
  if (lba_mode == 2 && dma == 1 && direction == 0) {
    cmd = ATA_CMD_READ_DMA_EXT;
  }
  if (lba_mode == 0 && dma == 0 && direction == 1) {
    cmd = ATA_CMD_WRITE_PIO;
  }
  if (lba_mode == 1 && dma == 0 && direction == 1) {
    cmd = ATA_CMD_WRITE_PIO;
  }
  if (lba_mode == 2 && dma == 0 && direction == 1) {
    cmd = ATA_CMD_WRITE_PIO_EXT;
  }
  if (lba_mode == 0 && dma == 1 && direction == 1) {
    cmd = ATA_CMD_WRITE_DMA;
  }
  if (lba_mode == 1 && dma == 1 && direction == 1) {
    cmd = ATA_CMD_WRITE_DMA;
  }
  if (lba_mode == 2 && dma == 1 && direction == 1) {
    cmd = ATA_CMD_WRITE_DMA_EXT;
  }

  ide_write(channel, ATA_REG_COMMAND, cmd);

  if (dma) {
    if (direction == 0) {

    } else {
      // DMA not supported
    }
  } else {
    if (direction == 0) {
      for (i = 0; i < numsects; i++) {
        if (err = ide_polling(channel, 1)) {
          return err;
        }
        __asm__ volatile("pushw %es");
        __asm__ volatile("rep insw" : : "c"(words), "d"(bus), "D"(buf));
        __asm__ volatile("popw %es");
        buf += (words * 2);
      }
    } else {
      for (i = 0; i < numsects; i++) {
        ide_polling(channel, 0);
        __asm__ volatile("pushw %ds");
        __asm__ volatile("rep outsw" : : "c"(words), "d"(bus), "S"(buf));
        __asm__ volatile("popw %ds");
        buf += (words * 2);
      }

      ide_write(channel, ATA_REG_COMMAND,
                (char[]){ATA_CMD_CACHE_FLUSH, ATA_CMD_CACHE_FLUSH,
                         ATA_CMD_CACHE_FLUSH_EXT}[lba_mode]);
      ide_polling(channel, 0);
    }
    return 0;
  }
}

int ide_read_sectors(unsigned char drive, unsigned char numsects,
                     unsigned int lba, uint32_t buf) {
  if (drive > MAX_IDE_DEVICES || devices[drive].reserved == 0) {
    printm("ide: read_sectors: error: drive not found\n");
    return -1;
  } else if (((lba + numsects) > devices[drive].size) &&
             (devices[drive].type == IDE_ATA)) {
    printm("ide: read_sectors: error: lba (0x%x) > drive size\n");
    return -2;
  } else {
    int err;
    if (devices[drive].type == IDE_ATA) {
      err = ide_ata_access(ATA_READ, drive, lba, numsects, buf);
    } else if (devices[drive].type == IDE_ATAPI) {
      printm("ide: read_sectors: atapi not supported\n");
      return -1;
    }
    ide_print_error(drive, err);
  }
}

int ide_write_sectors(unsigned char drive, unsigned char numsects,
                      unsigned int lba, uint32_t buf) {
  if (drive > MAX_IDE_DEVICES || devices[drive].reserved == 0) {
    printm("ide: write_sectors: error: drive not found\n");
    return -1;
  } else if (((lba + numsects) > devices[drive].size) &&
             (devices[drive].type == IDE_ATA)) {
    printm("ide: write_sectors: error: seeking to invalid position\n");
    return -2;
  } else {
    int err;
    if (devices[drive].type == IDE_ATA) {
      err = ide_ata_access(ATA_WRITE, drive, lba, numsects, buf);
    } else if (devices[drive].type == IDE_ATAPI) {
      err = -4;
    }
    ide_print_error(drive, err);
  }
}

void ide_rw_test() {
  int drive, lba = 0, numsects = 1;
  char buf[ATA_SECTOR_SIZE] = {0};
  for (int i = 0; i < MAX_IDE_DEVICES; i++) {
    if (strcmp(devices[i].model, "QEMU HARDDISK") == 0) {
      drive = i;
    } else {
      continue;
    }
  }

  struct test_example {
    char data[32];
    int data_id;
  };

  struct test_example e;
  e.data_id = 10012;
  strcpy(e.data, "Kinux Kernel");
  strcpy(buf, "IDE RW Test");
  ide_write_sectors(drive, numsects, lba, (uint32_t)buf);
  memset(buf, 0, sizeof(buf));
  memcpy(buf, &e, sizeof(e));
  ide_write_sectors(drive, numsects, lba + 1, (uint32_t)buf);
  printm("ide: rw_test: data written\n");

  memset(buf, 0, sizeof(buf));
  ide_read_sectors(drive, numsects, lba, (uint32_t)buf);
  printm("ide: rw_test: read data %s\n", buf);

  memset(buf, 0, sizeof(buf));
  ide_read_sectors(drive, numsects, lba + 1, (uint32_t)buf);
  memcpy(&e, buf, sizeof(e));
  printm("ide: rw_test: read struct: id=%d, data=%s\n", e.data_id, e.data);
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
      io_wait();
      ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
      io_wait();

      if (ide_read(i, ATA_REG_STATUS) == 0) {
        continue;
      }

      while (1) {
        status = ide_read(i, ATA_REG_STATUS);
        if ((status & ATA_SR_ERR)) {
          err = 1;
          break;
        }
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
        io_wait();
      }

      ide_read_buffer(i, ATA_REG_DATA, (uint32_t *)ide_buf, 128);
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
    if (devices[i].reserved == 1 && strlen(devices[i].model) > 0) {
      printm(" device %d:-\n", i);
      printm("  model: %s, ", devices[i].model);
      printm_dis_hdr = 1;
      printm("type: ");
      if (devices[i].type == IDE_ATA) {
        printm("ATA, ");
      } else if (devices[i].type == IDE_ATAPI) {
        printm("ATAPI, ");
      }

      printm("drive: %u, channel: %u, ", devices[i].drive, devices[i].channel);
      printm("size: %u sectors, %u bytes\n", devices[i].size,
             devices[i].size * ATA_SECTOR_SIZE);
      printm_dis_hdr = 0;
    }

  console_putc('\n');
}
