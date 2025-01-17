
#ifndef __SX1276_LORA_H__
#define __SX1276_LORA_H__
#include "sx1276.h"
#include "bsp.h"

/*******************************************************************
** SX1276 Internal registers Address                              **
*******************************************************************/
#define REG_LR_FIFO                                 0x00
// Common settings
#define REG_LR_OPMODE                               0x01
#define REG_LR_BANDSETTING                          0x04
#define REG_LR_FRFMSB                               0x06
#define REG_LR_FRFMID                               0x07
#define REG_LR_FRFLSB                               0x08
// Tx settings
#define REG_LR_PACONFIG                             0x09
#define REG_LR_PARAMP                               0x0A
#define REG_LR_OCP                                  0x0B
// Rx settings
#define REG_LR_LNA                                  0x0C
// LoRa registers
#define REG_LR_FIFOADDRPTR                          0x0D
#define REG_LR_FIFOTXBASEADDR                       0x0E
#define REG_LR_FIFORXBASEADDR                       0x0F
#define REG_LR_FIFORXCURRENTADDR                    0x10
#define REG_LR_IRQFLAGSMASK                         0x11
#define REG_LR_IRQFLAGS                             0x12
#define REG_LR_NBRXBYTES                            0x13
#define REG_LR_RXHEADERCNTVALUEMSB                  0x14
#define REG_LR_RXHEADERCNTVALUELSB                  0x15
#define REG_LR_RXPACKETCNTVALUEMSB                  0x16
#define REG_LR_RXPACKETCNTVALUELSB        0x          0x17
#define REG_LR_MODEMSTAT                            0x18
#define REG_LR_PKTSNRVALUE                          0x19
#define REG_LR_PKTRSSIVALUE                         0x1A
#define REG_LR_RSSIVALUE                            0x1B
#define REG_LR_HOPCHANNEL                           0x1C
#define REG_LR_MODEMCONFIG1                         0x1D
#define REG_LR_MODEMCONFIG2                         0x1E
#define REG_LR_SYMBTIMEOUTLSB                       0x1F
#define REG_LR_PREAMBLEMSB                          0x20
#define REG_LR_PREAMBLELSB                          0x21
#define REG_LR_PAYLOADLENGTH                        0x22
#define REG_LR_PAYLOADMAXLENGTH                     0x23
#define REG_LR_HOPPERIOD                            0x24
#define REG_LR_FIFORXBYTEADDR                       0x25
#define REG_LR_MODEMCONFIG3                         0x26
// end of documented register in datasheet
// I/O settings
#define REG_LR_DIOMAPPING1                          0x40
#define REG_LR_DIOMAPPING2                          0x41
// Version
#define REG_LR_VERSION                              0x42
// Additional settings
#define REG_LR_PLLHOP                               0x44
#define REG_LR_TCXO                                 0x4B
#define REG_LR_PADAC                                0x4D
#define REG_LR_FORMERTEMP                           0x5B
#define REG_LR_BITRATEFRAC                          0x5D
#define REG_LR_AGCREF                               0x61
#define REG_LR_AGCTHRESH1                           0x62
#define REG_LR_AGCTHRESH2                           0x63
#define REG_LR_AGCTHRESH3                           0x64

/*******************************************************************
** SX1276 initialisation register values definition               **
*******************************************************************/
#define DEF_LR_FIFO                                 0x00
// Common settings
#define DEF_LR_OPMODE                               0x09
#define DEF_LR_BANDSETTING                          0x00
#define DEF_LR_FRFMSB                               0x6C
#define DEF_LR_FRFMID                               0x80
#define DEF_LR_FRFLSB                               0x00
// Tx settings
#define DEF_LR_PACONFIG                             0x4F
#define DEF_LR_PARAMP                               0x09
#define DEF_LR_OCP                                  0x2B
// Rx settings
#define DEF_LR_LNA                                  0x20
// LoRa registers
#define DEF_LR_FIFOADDRPTR                          0x00
#define DEF_LR_FIFOTXBASEADDR                       0x80
#define DEF_LR_FIFORXBASEADDR                       0x00

#define DEF_LR_IRQFLAGSMASK                         0x00

#define DEF_LR_MODEMCONFIG1                         0x72
#define DEF_LR_MODEMCONFIG2                         0x70
#define DEF_LR_SYMBTIMEOUTLSB                       0x64
#define DEF_LR_PREAMBLEMSB                          0x00
#define DEF_LR_PREAMBLELSB                          0x08
#define DEF_LR_PAYLOADLENGTH                        0x01
#define DEF_LR_PAYLOADMAXLENGTH                     0xFF
#define DEF_LR_HOPPERIOD                            0x00

#define DEF_LR_MODEMCONFIG3                         0x00
// end of documented register in datasheet
// I/O settings
#define DEF_LR_DIOMAPPING1                          0x00
#define DEF_LR_DIOMAPPING2                          0x00
// Version

// Additional settings
#define DEF_LR_PLLHOP                               0x2D
#define DEF_LR_TCXO                                 0x09
#define DEF_LR_PADAC                                0x84


#define DEF_LR_AGCREF                               0x13
#define DEF_LR_AGCTHRESH1                           0x0E
#define DEF_LR_AGCTHRESH2                           0x5B
#define DEF_LR_AGCTHRESH3                           0xDB

/*******************************************************************
** SX1276 bit control definition                                  **
*******************************************************************/

/*!
 * RegFifo
 */

/*!
 * RegOpMode
 */
#define RFLR_OPMODE_LONGRANGEMODE_MASK              0x7F
#define RFLR_OPMODE_LONGRANGEMODE_OFF               0x00 // Default
#define RFLR_OPMODE_LONGRANGEMODE_ON                0x80

#define RFLR_OPMODE_ACCESSSHAREDREG_MASK            0xBF
#define RFLR_OPMODE_ACCESSSHAREDREG_ENABLE          0x40
#define RFLR_OPMODE_ACCESSSHAREDREG_DISABLE         0x00 // Default

#define RFLR_OPMODE_FREQMODE_ACCESS_MASK            0xF7
#define RFLR_OPMODE_FREQMODE_ACCESS_LF              0x08 // Default
#define RFLR_OPMODE_FREQMODE_ACCESS_HF              0x00

#define RFLR_OPMODE_MASK                            0xF8
#define RFLR_OPMODE_SLEEP                           0x00
#define RFLR_OPMODE_STANDBY                         0x01 // Default
#define RFLR_OPMODE_SYNTHESIZER_TX                  0x02
#define RFLR_OPMODE_TRANSMITTER                     0x03
#define RFLR_OPMODE_SYNTHESIZER_RX                  0x04
#define RFLR_OPMODE_RECEIVER                        0x05
// LoRa specific modes
#define RFLR_OPMODE_RECEIVER_SINGLE                 0x06
#define RFLR_OPMODE_CAD                             0x07

/*!
 * RegBandSetting
 */
#define RFLR_BANDSETTING_MASK                    0x3F
#define RFLR_BANDSETTING_AUTO                    0x00 // Default
#define RFLR_BANDSETTING_DIV_BY_1                0x40
#define RFLR_BANDSETTING_DIV_BY_2                0x80
#define RFLR_BANDSETTING_DIV_BY_6                0xC0

/*!
 * RegFrf (MHz)
 */

#define RFLR_FRFMSB_434_MHZ                         0x6C // Default
#define RFLR_FRFMID_434_MHZ                         0x80 // Default
#define RFLR_FRFLSB_434_MHZ                         0x00 // Default

#define RFLR_FRFMSB_863_MHZ                         0xD7
#define RFLR_FRFMID_863_MHZ                         0xC0
#define RFLR_FRFLSB_863_MHZ                         0x00
#define RFLR_FRFMSB_864_MHZ                         0xD8
#define RFLR_FRFMID_864_MHZ                         0x00
#define RFLR_FRFLSB_864_MHZ                         0x00
#define RFLR_FRFMSB_865_MHZ                         0xD8
#define RFLR_FRFMID_865_MHZ                         0x40
#define RFLR_FRFLSB_865_MHZ                         0x00
#define RFLR_FRFMSB_866_MHZ                         0xD8
#define RFLR_FRFMID_866_MHZ                         0x80
#define RFLR_FRFLSB_866_MHZ                         0x00
#define RFLR_FRFMSB_867_MHZ                         0xD8
#define RFLR_FRFMID_867_MHZ                         0xC0
#define RFLR_FRFLSB_867_MHZ                         0x00
#define RFLR_FRFMSB_868_MHZ                         0xD9
#define RFLR_FRFMID_868_MHZ                         0x00
#define RFLR_FRFLSB_868_MHZ                         0x00
#define RFLR_FRFMSB_869_MHZ                         0xD9
#define RFLR_FRFMID_869_MHZ                         0x40
#define RFLR_FRFLSB_869_MHZ                         0x00
#define RFLR_FRFMSB_870_MHZ                         0xD9
#define RFLR_FRFMID_870_MHZ                         0x80
#define RFLR_FRFLSB_870_MHZ                         0x00

#define RFLR_FRFMSB_902_MHZ                         0xE1
#define RFLR_FRFMID_902_MHZ                         0x80
#define RFLR_FRFLSB_902_MHZ                         0x00
#define RFLR_FRFMSB_903_MHZ                         0xE1
#define RFLR_FRFMID_903_MHZ                         0xC0
#define RFLR_FRFLSB_903_MHZ                         0x00
#define RFLR_FRFMSB_904_MHZ                         0xE2
#define RFLR_FRFMID_904_MHZ                         0x00
#define RFLR_FRFLSB_904_MHZ                         0x00
#define RFLR_FRFMSB_905_MHZ                         0xE2
#define RFLR_FRFMID_905_MHZ                         0x40
#define RFLR_FRFLSB_905_MHZ                         0x00
#define RFLR_FRFMSB_906_MHZ                         0xE2
#define RFLR_FRFMID_906_MHZ                         0x80
#define RFLR_FRFLSB_906_MHZ                         0x00
#define RFLR_FRFMSB_907_MHZ                         0xE2
#define RFLR_FRFMID_907_MHZ                         0xC0
#define RFLR_FRFLSB_907_MHZ                         0x00
#define RFLR_FRFMSB_908_MHZ                         0xE3
#define RFLR_FRFMID_908_MHZ                         0x00
#define RFLR_FRFLSB_908_MHZ                         0x00
#define RFLR_FRFMSB_909_MHZ                         0xE3
#define RFLR_FRFMID_909_MHZ                         0x40
#define RFLR_FRFLSB_909_MHZ                         0x00
#define RFLR_FRFMSB_910_MHZ                         0xE3
#define RFLR_FRFMID_910_MHZ                         0x80
#define RFLR_FRFLSB_910_MHZ                         0x00
#define RFLR_FRFMSB_911_MHZ                         0xE3
#define RFLR_FRFMID_911_MHZ                         0xC0
#define RFLR_FRFLSB_911_MHZ                         0x00
#define RFLR_FRFMSB_912_MHZ                         0xE4
#define RFLR_FRFMID_912_MHZ                         0x00
#define RFLR_FRFLSB_912_MHZ                         0x00
#define RFLR_FRFMSB_913_MHZ                         0xE4
#define RFLR_FRFMID_913_MHZ                         0x40
#define RFLR_FRFLSB_913_MHZ                         0x00
#define RFLR_FRFMSB_914_MHZ                         0xE4
#define RFLR_FRFMID_914_MHZ                         0x80
#define RFLR_FRFLSB_914_MHZ                         0x00
#define RFLR_FRFMSB_915_MHZ                         0xE4  // Default
#define RFLR_FRFMID_915_MHZ                         0xC0  // Default
#define RFLR_FRFLSB_915_MHZ                         0x00  // Default
#define RFLR_FRFMSB_916_MHZ                         0xE5
#define RFLR_FRFMID_916_MHZ                         0x00
#define RFLR_FRFLSB_916_MHZ                         0x00
#define RFLR_FRFMSB_917_MHZ                         0xE5
#define RFLR_FRFMID_917_MHZ                         0x40
#define RFLR_FRFLSB_917_MHZ                         0x00
#define RFLR_FRFMSB_918_MHZ                         0xE5
#define RFLR_FRFMID_918_MHZ                         0x80
#define RFLR_FRFLSB_918_MHZ                         0x00
#define RFLR_FRFMSB_919_MHZ                         0xE5
#define RFLR_FRFMID_919_MHZ                         0xC0
#define RFLR_FRFLSB_919_MHZ                         0x00
#define RFLR_FRFMSB_920_MHZ                         0xE6
#define RFLR_FRFMID_920_MHZ                         0x00
#define RFLR_FRFLSB_920_MHZ                         0x00
#define RFLR_FRFMSB_921_MHZ                         0xE6
#define RFLR_FRFMID_921_MHZ                         0x40
#define RFLR_FRFLSB_921_MHZ                         0x00
#define RFLR_FRFMSB_922_MHZ                         0xE6
#define RFLR_FRFMID_922_MHZ                         0x80
#define RFLR_FRFLSB_922_MHZ                         0x00
#define RFLR_FRFMSB_923_MHZ                         0xE6
#define RFLR_FRFMID_923_MHZ                         0xC0
#define RFLR_FRFLSB_923_MHZ                         0x00
#define RFLR_FRFMSB_924_MHZ                         0xE7
#define RFLR_FRFMID_924_MHZ                         0x00
#define RFLR_FRFLSB_924_MHZ                         0x00
#define RFLR_FRFMSB_925_MHZ                         0xE7
#define RFLR_FRFMID_925_MHZ                         0x40
#define RFLR_FRFLSB_925_MHZ                         0x00
#define RFLR_FRFMSB_926_MHZ                         0xE7
#define RFLR_FRFMID_926_MHZ                         0x80
#define RFLR_FRFLSB_926_MHZ                         0x00
#define RFLR_FRFMSB_927_MHZ                         0xE7
#define RFLR_FRFMID_927_MHZ                         0xC0
#define RFLR_FRFLSB_927_MHZ                         0x00
#define RFLR_FRFMSB_928_MHZ                         0xE8
#define RFLR_FRFMID_928_MHZ                         0x00
#define RFLR_FRFLSB_928_MHZ                         0x00

/*!
 * RegPaConfig
 */
#define RFLR_PACONFIG_PASELECT_MASK                 0x7F
#define RFLR_PACONFIG_PASELECT_PABOOST              0x80
#define RFLR_PACONFIG_PASELECT_RFO                  0x00 // Default

#define RFLR_PACONFIG_MAX_POWER_MASK                0x8F

#define RFLR_PACONFIG_OUTPUTPOWER_MASK              0xF0

/*!
 * RegPaRamp
 */
#define RFLR_PARAMP_TXBANDFORCE_MASK                0xEF
#define RFLR_PARAMP_TXBANDFORCE_BAND_SEL            0x10
#define RFLR_PARAMP_TXBANDFORCE_AUTO                0x00 // Default

#define RFLR_PARAMP_MASK                            0xF0
#define RFLR_PARAMP_3400_US                         0x00
#define RFLR_PARAMP_2000_US                         0x01
#define RFLR_PARAMP_1000_US                         0x02
#define RFLR_PARAMP_0500_US                         0x03
#define RFLR_PARAMP_0250_US                         0x04
#define RFLR_PARAMP_0125_US                         0x05
#define RFLR_PARAMP_0100_US                         0x06
#define RFLR_PARAMP_0062_US                         0x07
#define RFLR_PARAMP_0050_US                         0x08
#define RFLR_PARAMP_0040_US                         0x09 // Default
#define RFLR_PARAMP_0031_US                         0x0A
#define RFLR_PARAMP_0025_US                         0x0B
#define RFLR_PARAMP_0020_US                         0x0C
#define RFLR_PARAMP_0015_US                         0x0D
#define RFLR_PARAMP_0012_US                         0x0E
#define RFLR_PARAMP_0010_US                         0x0F

/*!
 * RegOcp
 */
#define RFLR_OCP_MASK                               0xDF
#define RFLR_OCP_ON                                 0x20 // Default
#define RFLR_OCP_OFF                                0x00

#define RFLR_OCP_TRIM_MASK                          0xE0
#define RFLR_OCP_TRIM_045_MA                        0x00
#define RFLR_OCP_TRIM_050_MA                        0x01
#define RFLR_OCP_TRIM_055_MA                        0x02
#define RFLR_OCP_TRIM_060_MA                        0x03
#define RFLR_OCP_TRIM_065_MA                        0x04
#define RFLR_OCP_TRIM_070_MA                        0x05
#define RFLR_OCP_TRIM_075_MA                        0x06
#define RFLR_OCP_TRIM_080_MA                        0x07
#define RFLR_OCP_TRIM_085_MA                        0x08
#define RFLR_OCP_TRIM_090_MA                        0x09
#define RFLR_OCP_TRIM_095_MA                        0x0A
#define RFLR_OCP_TRIM_100_MA                        0x0B  // Default
#define RFLR_OCP_TRIM_105_MA                        0x0C
#define RFLR_OCP_TRIM_110_MA                        0x0D
#define RFLR_OCP_TRIM_115_MA                        0x0E
#define RFLR_OCP_TRIM_120_MA                        0x0F
#define RFLR_OCP_TRIM_130_MA                        0x10
#define RFLR_OCP_TRIM_140_MA                        0x11
#define RFLR_OCP_TRIM_150_MA                        0x12
#define RFLR_OCP_TRIM_160_MA                        0x13
#define RFLR_OCP_TRIM_170_MA                        0x14
#define RFLR_OCP_TRIM_180_MA                        0x15
#define RFLR_OCP_TRIM_190_MA                        0x16
#define RFLR_OCP_TRIM_200_MA                        0x17
#define RFLR_OCP_TRIM_210_MA                        0x18
#define RFLR_OCP_TRIM_220_MA                        0x19
#define RFLR_OCP_TRIM_230_MA                        0x1A
#define RFLR_OCP_TRIM_240_MA                        0x1B

/*!
 * RegLna
 */
#define RFLR_LNA_GAIN_MASK                          0x1F
#define RFLR_LNA_GAIN_G1                            0x20 // Default
#define RFLR_LNA_GAIN_G2                            0x40
#define RFLR_LNA_GAIN_G3                            0x60
#define RFLR_LNA_GAIN_G4                            0x80
#define RFLR_LNA_GAIN_G5                            0xA0
#define RFLR_LNA_GAIN_G6                            0xC0

#define RFLR_LNA_BOOST_LF_MASK                      0xE7
#define RFLR_LNA_BOOST_LF_DEFAULT                   0x00 // Default
#define RFLR_LNA_BOOST_LF_GAIN                      0x08
#define RFLR_LNA_BOOST_LF_IP3                       0x10
#define RFLR_LNA_BOOST_LF_BOOST                     0x18

#define RFLR_LNA_RXBANDFORCE_MASK                   0xFB
#define RFLR_LNA_RXBANDFORCE_BAND_SEL               0x04
#define RFLR_LNA_RXBANDFORCE_AUTO                   0x00 // Default

#define RFLR_LNA_BOOST_HF_MASK                      0xFC
#define RFLR_LNA_BOOST_HF_OFF                       0x00 // Default
#define RFLR_LNA_BOOST_HF_ON                        0x03

/*!
 * RegFifoAddrPtr
 */
#define RFLR_FIFOADDRPTR                            0x00 // Default

/*!
 * RegFifoTxBaseAddr
 */
#define RFLR_FIFOTXBASEADDR                         0x80 // Default

/*!
 * RegFifoTxBaseAddr
 */
#define RFLR_FIFORXBASEADDR                         0x00 // Default

/*!
 * RegFifoRxCurrentAddr (Read Only)
 */

/*!
 * RegIrqFlagsMask
 */
#define RFLR_IRQFLAGS_RXTIMEOUT_MASK                0x80
#define RFLR_IRQFLAGS_RXDONE_MASK                   0x40
#define RFLR_IRQFLAGS_PAYLOADCRCERROR_MASK          0x20
#define RFLR_IRQFLAGS_VALIDHEADER_MASK              0x10
#define RFLR_IRQFLAGS_TXDONE_MASK                   0x08
#define RFLR_IRQFLAGS_CADDONE_MASK                  0x04
#define RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL_MASK       0x02
#define RFLR_IRQFLAGS_CADDETECTED_MASK              0x01

/*!
 * RegIrqFlags
 */
#define RFLR_IRQFLAGS_RXTIMEOUT                     0x80
#define RFLR_IRQFLAGS_RXDONE                        0x40
#define RFLR_IRQFLAGS_PAYLOADCRCERROR               0x20
#define RFLR_IRQFLAGS_VALIDHEADER                   0x10
#define RFLR_IRQFLAGS_TXDONE                        0x08
#define RFLR_IRQFLAGS_CADDONE                       0x04
#define RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL            0x02
#define RFLR_IRQFLAGS_CADDETECTED                   0x01



/*!
 * RegFifoRxNbBytes (Read Only)    //
 */


 /*!
 * RegRxHeaderCntValueMsb (Read Only)    //
 */


  /*!
 * RegRxHeaderCntValueLsb (Read Only)    //
 */


/*!
 * RegRxPacketCntValueMsb (Read Only)    //
 */


 /*!
 * RegRxPacketCntValueLsb (Read Only)    //
 */


 /*!
 * RegModemStat (Read Only)    //
 */
#define RFLR_MODEMSTAT_RX_CR_MASK                   0x1F
#define RFLR_MODEMSTAT_MODEM_STATUS_MASK            0xE0

/*!
 * RegPktSnrValue (Read Only)    //
 */


 /*!
 * RegPktRssiValue (Read Only)    //
 */


/*!
 * RegRssiValue (Read Only)    //
 */


 /*!
 * RegModemConfig1
 */
#define RFLR_MODEMCONFIG1_BW_MASK                   0x0F

#define RFLR_MODEMCONFIG1_BW_7_81_KHZ               0x00
#define RFLR_MODEMCONFIG1_BW_10_41_KHZ              0x10
#define RFLR_MODEMCONFIG1_BW_15_62_KHZ              0x20
#define RFLR_MODEMCONFIG1_BW_20_83_KHZ              0x30
#define RFLR_MODEMCONFIG1_BW_31_25_KHZ              0x40
#define RFLR_MODEMCONFIG1_BW_41_66_KHZ              0x50
#define RFLR_MODEMCONFIG1_BW_62_50_KHZ              0x60
#define RFLR_MODEMCONFIG1_BW_125_KHZ                0x70 // Default
#define RFLR_MODEMCONFIG1_BW_250_KHZ                0x80
#define RFLR_MODEMCONFIG1_BW_500_KHZ                0x90

#define RFLR_MODEMCONFIG1_CODINGRATE_MASK           0xF1
#define RFLR_MODEMCONFIG1_CODINGRATE_4_5            0x02
#define RFLR_MODEMCONFIG1_CODINGRATE_4_6            0x04 // Default
#define RFLR_MODEMCONFIG1_CODINGRATE_4_7            0x06
#define RFLR_MODEMCONFIG1_CODINGRATE_4_8            0x08

#define RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK       0xFE
#define RFLR_MODEMCONFIG1_IMPLICITHEADER_ON         0x01
#define RFLR_MODEMCONFIG1_IMPLICITHEADER_OFF        0x00 // Default

 /*!
 * RegModemConfig2
 */
#define RFLR_MODEMCONFIG2_SF_MASK                   0x0F
#define RFLR_MODEMCONFIG2_SF_6                      0x60
#define RFLR_MODEMCONFIG2_SF_7                      0x70 // Default
#define RFLR_MODEMCONFIG2_SF_8                      0x80
#define RFLR_MODEMCONFIG2_SF_9                      0x90
#define RFLR_MODEMCONFIG2_SF_10                     0xA0
#define RFLR_MODEMCONFIG2_SF_11                     0xB0
#define RFLR_MODEMCONFIG2_SF_12                     0xC0

#define RFLR_MODEMCONFIG2_TXCONTINUOUSMODE_MASK     0xF7
#define RFLR_MODEMCONFIG2_TXCONTINUOUSMODE_ON       0x08
#define RFLR_MODEMCONFIG2_TXCONTINUOUSMODE_OFF      0x00

#define RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK         0xFB
#define RFLR_MODEMCONFIG2_RXPAYLOADCRC_ON           0x04
#define RFLR_MODEMCONFIG2_RXPAYLOADCRC_OFF          0x00 // Default

#define RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK       0xFC
#define RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB            0x00 // Default


/*!
 * RegHopChannel (Read Only)
 */
#define RFLR_HOPCHANNEL_PLL_LOCK_TIMEOUT_MASK       0x7F
#define RFLR_HOPCHANNEL_PLL_LOCK_FAIL               0x80
#define RFLR_HOPCHANNEL_PLL_LOCK_SUCCEED            0x00 // Default

#define RFLR_HOPCHANNEL_PAYLOAD_CRC16_MASK          0xBF
#define RFLR_HOPCHANNEL_PAYLOAD_CRC16_ON            0x40
#define RFLR_HOPCHANNEL_PAYLOAD_CRC16_OFF           0x00 // Default

#define RFLR_HOPCHANNEL_CHANNEL_MASK                0x3F


/*!
 * RegSymbTimeoutLsb
 */
#define RFLR_SYMBTIMEOUTLSB_SYMBTIMEOUT             0x64 // Default

/*!
 * RegPreambleLengthMsb
 */
#define RFLR_PREAMBLELENGTHMSB                      0x00 // Default

/*!
 * RegPreambleLengthLsb
 */
#define RFLR_PREAMBLELENGTHLSB                      0x08 // Default

/*!
 * RegPayloadLength
 */
#define RFLR_PAYLOADLENGTH                          0x0E // Default

/*!
 * RegPayloadMaxLength
 */
#define RFLR_PAYLOADMAXLENGTH                       0xFF // Default

/*!
 * RegHopPeriod
 */
#define RFLR_HOPPERIOD_FREQFOPPINGPERIOD            0x00 // Default


/*!
 * RegDioMapping1
 */
#define RFLR_DIOMAPPING1_DIO0_MASK                  0x3F
#define RFLR_DIOMAPPING1_DIO0_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO0_01                    0x40
#define RFLR_DIOMAPPING1_DIO0_10                    0x80
#define RFLR_DIOMAPPING1_DIO0_11                    0xC0

#define RFLR_DIOMAPPING1_DIO1_MASK                  0xCF
#define RFLR_DIOMAPPING1_DIO1_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO1_01                    0x10
#define RFLR_DIOMAPPING1_DIO1_10                    0x20
#define RFLR_DIOMAPPING1_DIO1_11                    0x30

#define RFLR_DIOMAPPING1_DIO2_MASK                  0xF3
#define RFLR_DIOMAPPING1_DIO2_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO2_01                    0x04
#define RFLR_DIOMAPPING1_DIO2_10                    0x08
#define RFLR_DIOMAPPING1_DIO2_11                    0x0C

#define RFLR_DIOMAPPING1_DIO3_MASK                  0xFC
#define RFLR_DIOMAPPING1_DIO3_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO3_01                    0x01
#define RFLR_DIOMAPPING1_DIO3_10                    0x02
#define RFLR_DIOMAPPING1_DIO3_11                    0x03

/*!
 * RegDioMapping2
 */
#define RFLR_DIOMAPPING2_DIO4_MASK                  0x3F
#define RFLR_DIOMAPPING2_DIO4_00                    0x00  // Default
#define RFLR_DIOMAPPING2_DIO4_01                    0x40
#define RFLR_DIOMAPPING2_DIO4_10                    0x80
#define RFLR_DIOMAPPING2_DIO4_11                    0xC0

#define RFLR_DIOMAPPING2_DIO5_MASK                  0xCF
#define RFLR_DIOMAPPING2_DIO5_00                    0x00  // Default
#define RFLR_DIOMAPPING2_DIO5_01                    0x10
#define RFLR_DIOMAPPING2_DIO5_10                    0x20
#define RFLR_DIOMAPPING2_DIO5_11                    0x30

#define RFLR_DIOMAPPING2_MAP_MASK                   0xFE
#define RFLR_DIOMAPPING2_MAP_PREAMBLEDETECT         0x01
#define RFLR_DIOMAPPING2_MAP_RSSI                   0x00  // Default

/*!
 * RegVersion (Read Only)
 */

/*!
 * RegAgcRef
 */

/*!
 * RegAgcThresh1
 */

/*!
 * RegAgcThresh2
 */

/*!
 * RegAgcThresh3
 */

/*!
 * RegFifoRxByteAddr (Read Only)
 */

/*!
 * RegPllHop
 */
#define RFLR_PLLHOP_FASTHOP_MASK                    0x7F
#define RFLR_PLLHOP_FASTHOP_ON                      0x80
#define RFLR_PLLHOP_FASTHOP_OFF                     0x00 // Default

/*!
 * RegTcxo
 */
#define RFLR_TCXO_TCXOINPUT_MASK                    0xEF
#define RFLR_TCXO_TCXOINPUT_ON                      0x10
#define RFLR_TCXO_TCXOINPUT_OFF                     0x00  // Default

/*!
 * RegPaDac
 */
#define RFLR_PADAC_20DBM_MASK                       0xF8
#define RFLR_PADAC_20DBM_ON                         0x07
#define RFLR_PADAC_20DBM_OFF                        0x04  // Default

/*!
 * RegPll
 */
#define RFLR_PLL_BANDWIDTH_MASK                     0x3F
#define RFLR_PLL_BANDWIDTH_75                       0x00
#define RFLR_PLL_BANDWIDTH_150                      0x40
#define RFLR_PLL_BANDWIDTH_225                      0x80
#define RFLR_PLL_BANDWIDTH_300                      0xC0  // Default

/*!
 * RegPllLowPn
 */
#define RFLR_PLLLOWPN_BANDWIDTH_MASK                0x3F
#define RFLR_PLLLOWPN_BANDWIDTH_75                  0x00
#define RFLR_PLLLOWPN_BANDWIDTH_150                 0x40
#define RFLR_PLLLOWPN_BANDWIDTH_225                 0x80
#define RFLR_PLLLOWPN_BANDWIDTH_300                 0xC0  // Default

/*!
 * RegModemConfig3
 */
#define RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_MASK  0xF7
#define RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_ON    0x08
#define RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_OFF   0x00 // Default

#define RFLR_MODEMCONFIG3_AGCAUTO_MASK              0xFB
#define RFLR_MODEMCONFIG3_AGCAUTO_ON                0x04 // Default
#define RFLR_MODEMCONFIG3_AGCAUTO_OFF               0x00

/*!
 * RegFormerTemp
 */

 /*
================================================================================
--------------------------------MACRO DIFINITIONS-------------------------------
================================================================================
*/
typedef enum
{
    LORA_MODE_SLEEP              =   0x00,       //sleep mode
    LORA_MODE_STDBY              =   0x01,       //standby mode
    LORA_MODE_FSTX               =   0x02,       //frequency synthesis tx mode
    LORA_MODE_TX                 =   0x03,       //transmit mode
    LORA_MODE_FSRX               =   0x04,       //frequency synthesis rx mode
    LORA_MODE_RXC                =   0x05,       //receive continuous mode
    LORA_MODE_RXS                =   0x06,       //receive single mode
    LORA_MODE_CAD                =   0x07,       //channel activity dection mode
}LORA_CHIP_MODE;

typedef enum
{
	LORA_REG                     =   0x00,       //Access LoRa registers page 0x0D: 0x3F
	FSK_REG                      =   0x01,       //Access FSK registers page (in mode LoRa) 0x0D: 0x3F
}ACCESS_REG;

typedef enum
{
	CRC_ENABLE                   =   0x01,       //Enables the packet CRC generation
	CRC_DISABLE                  =   0x00,       //Disables the packet CRC generation
}CRC_EN;

typedef enum
{
	IMPLICITHEADER_ON            =   0x01,       //Enables the Implicit Header mode in LoRa
	IMPLICITHEADER_OFF           =   0x00,       //Disables the Implicit Header mode in LoRa
}IMPLICITHEADER_EN;

typedef enum
{
	TXCONTINUOUS_ON            =   0x01,       //Enables Tx continuous model
	TXCONTINUOUS_OFF           =   0x00,       //Disables Tx continuous model
}TXCONTINUOUS_EN;

//typedef enum
//{
//	RXSINGLE_ON                  =   0x01,       //Enables Rx single instead of Rx continuous
//	RXSINGLE_OFF                 =   0x00,       //Disables Rx single instead of Rx continuous
//}RXSINGLE_EN;
//
//typedef enum
//{
//	FREQHOP_ON                   =   0x01,       //Enables the frequency hopping
//	FREQHOP_OFF                  =   0x00,       //Disables the frequency hopping
//}FREQHOP_EN;
typedef enum
{
	LDOPTIMIZE_ON                =   0x01,       //Enables the low datarate optimization
	LDOPTIMIZE_OFF               =   0x00,       //Disables the low datarate optimization
}LDOPTIMIZE_EN;

//typedef enum
//{
//    FLAG_RXTIMEOUT               =   0x07,       //Timeout interrupt
//    FLAG_RXDONE                  =   0x06,       //Packet reception complete interrupt
//    FLAG_PAYLOADCRCERROR         =   0x05,       //Payload CRC error interrupt
//    FLAG_VALIDHEADER             =   0x04,       //Valid header received in Rx
//    FLAG_TXDONE                  =   0x03,       //FIFO Payload transmission complete interrupt
//    FLAG_CADDONE                 =   0x02,       //CAD complete
//    FLAG_FHSSCHANGECHANNEL       =   0x01,       //FHSS change channel interrupt
//    FLAG_CADDETECTED             =   0x00,       //Valid Lora signal detected during CAD operation
//}FLAG_CLEAR;
typedef enum
{
    RF_IDLE,  //
    RF_BUSY,
    RF_RX_DONE,  //receive successed
//    RF_TX_DONE,
}RF_STATE;
typedef enum
{
    RFLR_STATE_IDLE,
    RFLR_STATE_RX_INIT,
    RFLR_STATE_RX_RUNNING,
    RFLR_STATE_RX_DONE,
//    RFLR_STATE_RX_TIMEOUT,
//    RFLR_STATE_TX_INIT,
//    RFLR_STATE_TX_RUNNING,
//    RFLR_STATE_TX_DONE,
//    RFLR_STATE_TX_TIMEOUT,
//    RFLR_STATE_CAD_INIT,
//    RFLR_STATE_CAD_RUNNING,
}RFLR_STATE;

/*
================================================================================
-------------------------EXPORT FUNCTIONS OF DRIVER-----------------------------
================================================================================
*/
/* Set the AccessSharedReg 0x0D : 0x3F */
void SX1276_LoRa_SetReg( ACCESS_REG access_reg );

/* Put the SX1276 into another work mode */
void SX1276_LoRa_SetMode( LORA_CHIP_MODE lora_chip_mode );

/* Set the new Signal Bandwidth value */
void SX1276_LoRa_SetBW( INT8U bw );

/* Set the new Spreading Factor value */
void SX1276_LoRa_SetSpreadingFactor( INT8U sf );

/* Set the new Error Coding value */
void SX1276_LoRa_SetErrorCoding( INT8U ec );

/* Enables/Disables the packet CRC generation */
void SX1276_LoRa_SetCRCEn( CRC_EN crc_en );

/* Enables/Disables the Implicit Header mode in LoRa */
void SX1276_LoRa_SetImplicitHeaderEn( IMPLICITHEADER_EN implicitheader_en );

/* Enables/Disables Tx continuous model */
void SX1276_LoRa_SetTxContinuousEn( TXCONTINUOUS_EN txcontinous_en );

/* Enables/Disables Rx single instead of Rx continuous */
//void SX1276_LoRa_SetRXSingleEn( RXSINGLE_EN rxsingle_en );

/* Enables/Disables the frequency hopping */
void SX1276_LoRa_SetFreqHoppingPeriod( INT8U hopping_period );

/* Set payload length */
void SX1276_LoRa_SetPayloadLength( INT8U payload_tength );

/* Set Symbol Timeout based on symbol length */
void SX1276_LoRa_SetSymbTimeout( INT16U value );

/* Configure the device to optimize low datarate transfers */
void SX1276_LoRa_SetLowDatarateOptimize( LDOPTIMIZE_EN ldoptimize_en);

/* Initializes the SX1276 */
void SX1276_LoRa_Init( void );

/* Clear the flag */
//void SX1276_LoRa_ClearFlag( FLAG_CLEAR flag_clear );

/* Send packet via the SX1276 */
void SX1276_LoRa_SendPacket( INT8U *buffer, INT8U size );

/* Sets the radio in Rmode */
void SX1276_LoRa_StartRx( void );

/* Gets a copy of the current received buffer */
INT8U SX1276_LoRa_GetRxPacket( INT8U *buffer );

/* Process the LoRa modem Rx state machines depending on the SX1276 operating mode */
INT8U SX1276_LoRa_RxProcess( void );

/* Check if implicit header mode in LoRa in enabled or disabled */
INT8U SX1276_LoRa_GetImplicitHeaderOn( void );

/* Special setting when Spreading Factor = 6 */
void SX1276_LoRa_SetSF6( void );

/* Set the Datarate of SX1276 in LoRa model */
void SX1276_LoRa_SetDataRate( INT8U rate );

/* Flush the FIFO in LoRa model */
void SX1276_LoRa_FlushFifo( void );

/*the preamble count*/
void SX1276_Lora_SetPreambleSize( INT16U cnt );

#endif //__SX1276_LORA_H__