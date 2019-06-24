// �������� ��������.

#define RegFifo 	0x00
#define RegOpMode 	0x01 // ��������� ������ ������ � ������ ���

// �������� ��� ��������� ������� 868, 915. ���������� ������ � ������ ��� ��� ��������
#define RegFrMsb 0x06 // frf (23:16)
#define RegFrMid 0x07 // frf (15:8)
#define RegFrLsb 0x08 // frf (7:0)

// �������� RF, ��� ��������� ��� �������� � �����
#define RegPaConfig 0x09
#define RegPaRamp 	0x0a
#define RegOcp 		0x0b
#define RegLna 		0x0c

// �������� FIFO
#define RegFifoAddrPtr 			0x0d
#define RegFifoTxBaseAddr 		0x0e
#define RegFifoRxBaseAddr 		0x0F
#define RegFifoRxCurrentAddr 	0x10 // ��������� ����� (� ������ ������) ���������� ����������� ������
#define RegIrqFlagsMask			0x11
#define RegIrqFlags 			0x12
#define RegRxNbBytes			0x13 // ���������� ���� �������� �������� ���������� ����������� ������
#define RegFifoRxByteAddr 		0x25 //������� �������� ��������� ������ ������ RX (����� ���������� ����������� ��������� �������)

#define RegRxHeaderCntValueMsb 0x14 // 15:8 ������� �������� �������� , ����������� ��� �������� � ������ �����
#define RegRxHeaderCntValueLsb 0x15 // 7:0

#define RegModemStat	0x18 // Modem status
// ����� �������������� ��������� �� 0x12 - 0x1c

// �������� ��������� �������� ������ (�������� � ������������ ������������� ������)
#define RegModemConfig1 	0x1d
#define RegModemConfig2 	0x1e
#define RegSymbTimeoutLsb 	0x1f
#define RegModemConfig3		0x26

#define PpmCorrection 0x27 //
#define RegDetectOptimize 0x31
#define RegDetectionThreshold 0x37
#define RegInvertIQ 0x33
#define RegSyncWord 0x39

// Preamle
#define RegPreambleMsb 	0x20 // 15:8
#define RegPreambleLsb 	0x21 // 7:0

#define RegPayloadLength 	0x22
#define RegMaxPayloadLength 0x23 // ������ ������������ ������ ������
#define RegHopPeriod 		0x24

// ����� ��� lora � fsk ����������� ��������
#define RegDioMapping1 	0x40 // ����������� ��������� DIO0 � DIO3
#define RegDioMapping2 	0x41 // ����������� ��������� DIO4 � DIO5, ������� ClkOut
#define RegVersion 	   	0x42 // Semtech ID, ���������� ���������� �������
#define RegTcxo 		0x4b // ��������� ����� TCXO ��� XTAL // �������� ��������� ���������
#define RegPaDac 		0x4d // ����� ������� ��������� �������� PA
#define RegFormerTemp 	0x5b // ����������� ����������� �� ����� ���������� ���������� IQ
#define RegAgcRef 		0x61 //

//Adjustment of the AGC thresholds/ ����������� ������� AGC
#define RegAgcThresh1 0x62
#define RegAgcThresh2 0x63
#define RegAgcThresh3 0x64
#define RegPll 0x70

// ������
#define MODE_SLEEP 			  0b00000000 // ������ ����� (������ � ���� ������ ����� ���������� ����� ������ ����\���)
#define MODE_STDBY			  0b00000001 // ����� �������� . (��������� ������� 868)
#define MODE_TX_FSTX		  0b00000010 //
#define MODE_TX				  0b00000011 // ��������� ����� , ����� �������� ������� � �����  ��������
#define MODE_RX_FSRX		  0b00000100 //
#define MODE_RXCONTINUOUS	  0b00000101 // ���������� ������������ ��� ���������� ������
#define MODE_RXSINGLE		  0b00000110 // ��������� ����� , ����� ��������� � ��������
#define MODE_CAD			  0b00000111 // ��������� �����, �������� ���� ����� ���������� ���������....

