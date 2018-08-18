## Linux常用命令手册		
```
NO	分类	PS1	命令名	 用法及参数							功能注解								对应章节					
1	文件管理	#	ls			ls -a							列出当前目录下的所有文件，包括以.头的隐含文件													
	文件管理	#	ls			ls -l或ll				 列出当前目录下文件的详细信息													
	文件管理	#	pwd			pwd							  查看当前所在目录的绝对路经													
	文件管理	#	cd			cd ..							回当前目录的上一级目录													
	文件管理	#	cd			cd -							回上一次所在的目录													
	文件管理	#	cd			cd ~ 或 cd				  回当前用户的宿主目录													
	文件管理	#	cd			cd ~用户名					回指定用户的宿主目录													
2	文件管理	#	mkdir		mkdir 目录名				创建一个目录													
	文件管理	#	mkdir		mkdir –p					 递归式去创建一些嵌套目录													
	文件管理	#	rmdir		Rmdir 空目录名			删除一个空目录													
3	文件管理	#	rm			rm 文件名 文件名		删除一个文件或多个文件													
	文件管理	#	rm			rm -rf 非空目录名	 递归删除一个非空目录下的一切，不让提式-f													
4	文件管理	#	cat			cat文件名					 一屏查看文件内容													
5	文件管理	#	more			more文件名				分页查看文件内容													
6	文件管理	#	less			less 文件名			可控分页查看文件内容													
7	文件管理	#	grep			grep字符 文件名		根据字符匹配来查看文件部分内容													
8	文件管理	#	mv			mv 路经/文件  /经/文件			移动相对路经下的文件到绝对路经下													
	文件管理	#	mv			mv 文件名 新名称					 在当前目录下改名													
9	文件管理	#	cp			cp /路经/文件  ./					 移动绝对路经下的文件到当前目录下													
10	文件管理	#	find			find 路经 -name “字符串”		查找路经所在范围内满足字符串匹配的文件和目录													
11	文件管理	#	ln			ln 源文件 链接名						 创建当前目录源文件的硬链接													
	  示例   ln /home/test /usr/test1				   	     在/usr下建立/home/test的硬链接																				
12	文件管理	#	ln			ln -s a b							     创建当前目录下a的符号链接b													
13	文件管理	#	touch			touch file1 file2				 创建两个空文件													
14	磁盘管理	#	df			df						           	 用于报告文件系统的总容量，使用量，剩余容量。													
15	磁盘管理	#	du			du -b /home							   查看目前/HOME目录的容量(k)及子目录的容量(k)。													
16	磁盘管理	#	fdisk			fdisk -l							   查看系统分区信息													
17	磁盘管理	#	fdisk			fdisk /dev/sdb					 为一块新的SCSI硬盘进行分区													
18	磁盘管理	#	mkfs.ext3			Mkfs.ext3 /dev/sdb1	 为第一块SCSI硬盘的第一主分区格式化成ext3的文件系统													
		         # mkfs.ext2			Mkfs.ext2/dev/sdb2 	格式化成ext2文件系统													
19	磁盘管理	#	mount			mount -t 文件系统类型 设备路经  访问路经																				
	  磁盘管理	#				文件系统类型																				
					        	 Iso9660							光驱文件系统													
						          vfat							Fat文件系统(windows)													
	 挂载光驱	#	mount –t iso9660 /dev/cdrom /mnt/cdrom  																							
	 挂载FAT	#	mount –t vfat /dev/hda5 /mnt/cdrom 			挂第一个ide的第五个逻辑分区										
17	磁盘管理	#	Umount /mnt/cdrom  										卸载/mnt/cdrom为空													
18	文件权限	#	chmod			
               chmod u+s file							为file的属主加上特殊权限													
						   chmod g+r file							为file的属组加上读权限													
						   chmod o+w file							为file的其它用户加上写权限													
						   chmod a-x file							为file的所有用户减去执行权限													
	             chmod 765 file  为file的属主设为完全权限，属组设成读写权，其它用户具有读和执心权限																									
19	文件权限	#	chown					chown root /home							把/home的属主改成root用户											
20	文件权限	#	chgrp					chgrp root /home							把/home的属组改成root组											
21	打印管理	#	redhat-config-printer-tui										进入安装打印机界面											
22	打印管理	#	lp					  lp –d hptr file							  打印file到hptr的打印机上											
23	打印管理	#	lpq					  Lpq –P 打印机名							   查看打印机的状态											
24	打印管理	#	lprm				  Lprm –P 打印机名 a						 删除打印机内的打印作业											
25	打印管理	#	disable				Disable –r “changing paper” HPtr 		禁用打印机并提示原因						
26	打印管理	#	enable			  Enable HPtr					      	重新启用被禁用的												
27	用户管理	#	useradd				Useradd						          创建一个新的用户												
28	用户管理	#	groupadd		  Groupadd 组名						   创建一个新的组												
29	用户管理	#	passwd				Passwd 用户名						    为用户创建密码												
30	用户管理	#	Passwd -d		  Passwd -d用户名						删除用户密码也能登陆												
31	用户管理	#	Passwd -l			Passwd -l用户名						锁定账号密码												
32	用户管理	#	Passwd -u			Passwd -u用户名						解锁账号密码												
33	用户管理	#	Passwd -S			Passwd -S用户名						查询账号密码												
34	用户管理	#	Usermod -l		Usermod -l 新用户名 老用户名			为用户改名								
35	用户管理	#	Usermod -L		Usermod -L 要锁定用户名						锁定用户登陆								
36	用户管理	#	Usermod -U		Usermod –U解锁用户名							解锁用户登陆								
37	用户管理	#	Usermod -u		Usermod –u 501用户名							改变用户UID								
38	用户管理	#	Userdel				Userdel–r 用户名									删除用户一切								
39	用户管理	#	Groupmod -n		Groupmod –n新用户名 老用户名	  	为组改名								
40	用户管理	#	Groupmod -g		Groupmod –g 501 组名						改变组GID								
41	用户管理	#	groupdel			Groupdel组名    先应删它的用户		删除组								
42	用户管理	#	gpasswd -a		gpasswd -a 用户名 组名						增加用户到组								
43	用户管理	#	Id 				   id   用户名								    		查用户信息								
44	软件管理	#	rpm -qa		  rpm –qa | less								    查询已安装RPM								
45	软件管理	#	rpm –qa | grep ftp								           查询指定RPM								
46	软件管理	#	rpm -q		 rpm -q 已安装的RPM包						查是否安装								
47	软件管理	#		 rpm -q telnet-server								 查看telnet服务器包								
48	软件管理	#	rpm -qi					rpm –qi 软件包名称					 查看软件的描述信息								
49	软件管理	#	rpm -ql					rpm –ql软件包名称					 查询软件包的文件列表								
50	软件管理	#	rpm -qf					rpm –qf软件包名称					查询某个文件所属的软件包								
51	软件管理	#	rpm -qp					rpm –qp软件包全名					查询未安装的软件包信息								
52	软件管理	#	rpm -e					rpm –e 软件包名称					删除具体的软件包								
53	软件管理	#	rpm -U					rpm –Uvh软件包全名					升级软件包并显示过程								
54	软件管理	#	rpm -ivh					rpm –ivh 软件包全名			安装软件包并显示过程								
55	软件管理	#	rpm -V					rpm –V软件包名称						验证软件包的大小，类型等								
56	软件管理	#	tar 					-c 创建包 –x 释放包  -v 显示命令过程 –z 代表压缩包																		
57	软件管理	#	tar -cf					tar –cvf benet.tar /home/benet			把/home/benet目录打包								
58	软件管理	#	tar -czf				tar –zcvf benet.tar.gz /mnt					把目录打包并压缩								
59	软件管理	#	tar –tf					tar –tf benet.tar										看非压缩包的文件列表								
60	软件管理	#	tar –tzf				tar –tf benet.tar.gz								看压缩包的文件列表								
61	软件管理	#	tar –xf					tar –xf benet.tar										非压缩包的文件恢复								
62	软件管理	#	tar –zxvf				tar –zxvf benet.tar.gz							压缩包的文件解压恢复								
63	软件管理	#	tar -jxvf				tar –jxvf benet.tar.bz2																		
64	软件管理	#	diff					  diff file1 file2 > 补丁名.patch			为新旧文件生成补丁文件								
65	软件管理	#	diff				  	diff file1 file2										比较两个文件的区别								
66	软件管理	#	Patch 					Patch 										          打补丁								
67	软件管理	#	./configure  必须在已解压目录下											编译前配置								
68	软件管理	#	make															                  编译								
69	软件管理	#	make install															         安装编译好的源码包								
70	启动管理	#	reboot							Init 6							         	重启LINUX系统								
71	启动管理	#	Halt		Init 0						Shutdown –h now					关闭LINUX系统								
72	启动管理	#	runlevel															           显示系统运行级								
73	启动管理	#	Init [0123456] 															     改变系统运行级,7种								
74	启动管理	#	Chkconfig –-list [服务名称]												查看服务的状态								
75	启动管理	#	Chkconfig –-level <运行级> <服务名> on|off|set 		设置服务的启动状态						
76	启动管理	#	Chkconfig <服务名> on|off|set										设置非独立服务启状态						
77	进程管理	#	Top动态			Ps-aux静态						进程树pstree		 查看系统进程							
78	进程管理	#	程序名 &				后台运行程序																	
79	进程管理	#	fg						把后台运行的进程调回前台																	
80	进程管理	#	bg						把前台运行进程调到后台																	
81	进程管理	#	renice						Renice +1 180 								把180号进程的优先级加1									
82	进程管理	#	kill						Kill PID 								        终止某个PID进程									
83	进程管理	#	at						at 5pm + 3 days								   指定三天后下午5:00执行/bin/ls									
									/bin/ls																	
84	进程管理	#	crontab						Crontab -e								   用VI的形式来编辑自动周期性任务									
85	进程管理	#	crontab						Crontab -l							    	查看自动周期性任务									
86	进程管理	#	crontab						Crontab -r								   删除自动周期性任务									
87	进程管理	#	crond						Service crond <start|stop|restart|status>																	
	马上启动自动周期性服务						Service crond <启动|停止|重启|状态>																			
							(下载源码就到源码网:www.codepub.com）																			
	实现磁盘配额			(注安装LINUX时建立/home分区） 																						
				目标：对用户zhao在/home目录上实现soft limit为5k,hard limit 为10k的磁盘配额 																						
				实现步骤： 																						
				1. 修改包含/home的行，  #vi /etc/fstab， 改为：defaults,usrquota。也就是增加usrquota项。然后保存退出。 																						
				2、卸载/home目录                           #umount /home 																						
				3. 挂接/home目录                           #mount /home 																						
				4、增加用户zhao                            #useradd zhao  																						
				5、修改密码                                #passwd zhao  																						
				6、生成关于/home目录的quota信息             # quotacheck -cmug /home																						
				                                          #quotacheck -vu /home  																						
				7、查看所有用户的信息                       #repquota -au 																						
				8、设置配额                                #edquota -u zhao 																						
				将soft 和hard 分别改为5和10 																						
				9、保存并退出                               #wq!  																						
				10、修改时间                                #edquota -t  																						
				11、                                       #wq! 																						
				12.开启/home上的磁盘配额功能                 #quotaon /home 																						
				13.查询配额                                 #quota -u zhao																						
				14.验证配额                                 #su - zhao																						
				                                           $touch myfile																						
```
