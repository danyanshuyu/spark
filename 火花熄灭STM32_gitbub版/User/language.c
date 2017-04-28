#include "stm32f10x_lib.h"
#include "language.h"

u8 Lang_Flag = 0;   //FlagΪ0 ��ʾ������ʾ��
                    //FlagΪ1 ��ʾӢ����ʾ��
                    //��Ӧ�ַ���ָ���ά����ĵ�һά��ֵ

char *Content[2][120]={{  //  0      1      2      3      4      5      6      7      8      9            																	
					 /*0*/ "����","��ʼ","����","��ţ�","��������","L001 ����װ��","DG001 ���� 103","L002 ����������","��ʼִ��Ϩ�����","������ȷ�Ͽ���",
					 /*1*/ "�������ر����","Ϩ�����","������ȷ�Ϲر�","����","��ⲻ��������","�������ط�������","��������","δ�ܼ�⵽������","���鴫�����Ƿ��ѽ���","��鿴�������Ƿ�����",
					 /*2*/ "�ر�","�ر�̽����","�ر�Ϩ����","ϵͳ","  1   ��������","  2   �鿴������Ϣ","  3   �رղ���","  4   ����Ԫ������","  5   ά��ģʽ","  6   ��װ����",	   
					 /*3*/ "  7   ϵͳ��������","  8   �ر�ϵͳ","������С���̡�����","��ʽΪ��XXXX��","611 �𻨼��ޣ����λ��� (��λ������)","612 �𻨼��ޣ��������� (��λ������)","613 �𻨼��ޣ���ʱ�� (��λ����)","614 ʱ�伫�� (��λ����)","615 Ϩ��ʱ�� (��λ����)","��ʽΪ��X������XX����XXX��",
					 /*4*/ "��ǰֵ","�޸�ֵ","61 ̽ͷ�����","62 ��ѹ����","63 ��������","64 ϵͳ�رղ���","7 ����ϵͳ����","71 ��������","72 ��������","73 ����ʱ��",
					 /*5*/ "74 ��������","75 ɾ�����ݲ���","76 �����Աȶ�","77 �����ڲ�����","78 ���¿�����","79 ���ÿ�����","������С���̡���������","��ʽΪ��XX.XX.XX��","��ǰΪ","�޸�Ϊ",
					 /*6*/ "������ԭ����","21 �鿴������Ϣ","22 �鿴������Ϣ","23 �鿴�ر���Ϣ","��ʷ��Ϣ","751 ɾ��������ʷ��Ϣ","752 ɾ��������ʷ��Ϣ","753 ɾ���ر���ʷ��Ϣ","ȷ��ɾ��ȫ��������Ϣ��","������š���ȡ�����������š���ȷ��",
					 /*7*/ "��ɾ��ȫ��������Ϣ��","ȷ�ϲ��Դ�����Ԫ����","���ڽ��в��ԡ�����","���Եȣ�","�˹����д�����","���ݳ����涨������","�뼰ʱ��������","611 �𻨼��ޣ����λ���","612 �𻨼��ޣ���������","613 �𻨼��ޣ���ʱ��",
					 /*8*/ "614 ʱ�伫��","615 Ϩ��ʱ��","����    ","       ������ˮ","  �Բ�����û��Ȩ�޷��ʣ�","������ȷ��","���ٴε����С���̡�","������ݲ�ȷ���Ի��Ȩ��","������ݲ�ȷ����ȡ��Ȩ��","�����������������",
					 /*9*/ "��ϲ���ѻ�Ȩ��","��ȡ��Ȩ��","��ʽ��������������","���������룬��ʽΪ��XXXX��","����������","������ݲ�ȷ���Ա�������","�ѱ�������","771   �򿪰���������","772   �رհ���������","DG001  ����������",
					/*10*/ "DG001  ��������������","DG002  ������δ֪","��ʼ����","�������ϣ����Ų飡","�����ɹ�","���","ȷ��ɾ��ȫ��������Ϣ��","��ɾ��ȫ��������Ϣ��","31 ̽ͷ�鿪�ز���","312   ��̽ͷ��",
					/*11*/ "311   �ر�̽ͷ��","̽ͷ�ѱ��ر�","       ��Ϩ��","616 ����ʱ��","616 ����ʱ�� (��λ����)","","","","","",
					   },
                       {
					 /*0*/ "Alarm","Begining","End","Nr��","Alarminfo","L001 Filter","DG001 Sparks 103","L002 Cyclone","Extinguishment start","Sensor open clear",
					 /*1*/ "Sensor closed made","Extinguishment end","Sensor closed clear","Fault","Flow switch signal too long","Get open switch fault","Faultinfo","The flow switch signal continues after","extinguishing.The flow switch signal","continues for other extinguishing systems",
					 /*2*/ "Disable","Cutoff detector group","Cutoff extinguishment group","System","1 Enter code","2 View stored data","3 Deal with disabling","4 Function test components","5 Deal with maintenance mode","6 Deal with installation parameters",	   
					 /*3*/ "7 Deal with system parameters","8 Disable system","Please click keyboard to enter","The format is��XXXX��","611 Counter threshold,counter sparks","612 Counter threshold,sustained counter sparks","613 Counter threshold,counter sparks[sec.]","614 Time threshold[sec.]","615 Extinguishment time[sec.]","The format is��X��,��XX��or��XXX��",
					 /*4*/ "Current","Change to","61 Parameters for detector group","62 Parameters for pump system","63 Parameters for frost protection","64 Parameters for system disable","7 Adjust system parameters","71 Change langusge","72 Change date","73 Change time",
					 /*5*/ "74 Change pass code","75 Delete data memory","76 Set display contrast","77 Set internal horn","78 Control console update","79 Control console configuration","Please click keyboard to enter","The format is��XX.XX.XX��","Current","Change to",
					 /*6*/ "Enter the old password","21 View data of alarm messages","22 View data of fault messages","23 View data of disable messages","History","751 Delete alarm history","752 Delete fault history","753 Delete disable history","Confirm to delete all alerts?","Press��x��to cancel���̡�to confirm",
					 /*7*/ "All alerts deleted","Confirm to test components?","Testing...","Please wait on","Remain to be developed","Too much data","Please clean up immediately","611 Counter threshold,counter sparks","612 Counter threshold,sustained counter sparks","613 Counter threshold,counter sparks",
					 /*8*/ "614 Time threshold","615 Extinguishment time","Sparks    ","       Spraying","Sorry��you do not have access rights!","code right!","click keyboard again","clear and confirm to get access","clear and confirm to cancel access","code wrong ,retry",
					 /*9*/ "get permission","Cancel permission","wrong,retry","enter new code,format is��XXXX��","code entered","clear and confirm to save","password saved","771   Open the Buzzer","772   Close the Buzzer","DG001  Sensor fault",
					/*10*/ "DG001  Sensor works properly","DG002  Sensor unknown","","","","","","","","",
					/*11*/ "","","","","","","","","","",
					  }};
