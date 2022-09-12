void mainmenu(int sd);
int  login(int sd);
int  signup(int sd);
void View_Bookings(int sd, int id, int type);
int  Adminmenu(int sd, int id);
int  Usermenu(int sd, int id, int type);
char *ACCOUNT[3] = {
        "db/accounts/customer",
        "db/accounts/agent",
        "db/accounts/admin"
};

int acc_size = sizeof(struct account);
int trn_size = sizeof(struct train);
int book_size = sizeof(struct bookings);

int login(int sd) {
        int  acc_no, type,fd, v = 1, n = 0, c = 0;
        char password[20];
        struct account AC;
        read(sd, & type, sizeof(int));
        if (type == 4) return 0;

        read(sd, & acc_no, sizeof(acc_no));
        read(sd, & password, sizeof(password));

        if ((fd = open(ACCOUNT[type - 1], O_RDWR)) == -1)
                perror("open");
        lseek(fd, (acc_no - 1) * acc_size, SEEK_CUR);
        struct flock lock;
	
	lock.l_start = (acc_no-1)*acc_size;
	lock.l_len = acc_size;
	lock.l_whence = SEEK_SET;
	lock.l_pid = getpid();

        switch (type) {

        case 1: {        
		lock.l_type = F_WRLCK;
		fcntl(fd,F_SETLKW, &lock);
                read(fd, & AC, acc_size);
                
                if (AC.id == acc_no) {
                        if (!strcmp(AC.pass, password)) {
                                write(sd, & v, sizeof(v));
                                while (Usermenu(sd, AC.id, type) != -1);
                                c = 1;
                        }
                }
                lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
                close(fd);
                if (c)
                        return 3;        }
        break;

        case 2: {
       	lock.l_type = F_RDLCK;
		fcntl(fd,F_SETLKW,&lock);
                read(fd, & AC, acc_size);
                lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
                close(fd);

                if (AC.id == acc_no) {
                        if (!strcmp(AC.pass, password)) {
                                write(sd, & v, sizeof(v));
                                while (Usermenu(sd, AC.id, type) != -1);
                                return 3;
                        }
                }
                if (c)
                        return 3;        }
        break;

        case 3: {
               lock.l_type = F_WRLCK;
		fcntl(fd,F_SETLKW, &lock);
                read(fd, &AC, acc_size);
                if (AC.id == acc_no) {
                        if (!strcmp(AC.pass, password)) {
                                write(sd, & v, sizeof(v));
                                while (Adminmenu(sd, AC.id) != -1);
                                c = 1;
                        }
                }
               lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
                close(fd);
                if (c)
                        return 3;        }
        break;
        }
        write(sd, & n, sizeof(n)); 
        return 3;
}

int signup(int sd) {
        int type, fd, acc_no = 0;
        char password[20], name[10];
        struct account acc;

        read(sd, & type, sizeof(int)); 
        if (type == 4) return 0; //back

        read(sd, & name, sizeof(name));
        read(sd, & password, sizeof(password));

        if ((fd = open(ACCOUNT[type - 1], O_RDWR)) == -1) 
                perror("open");
        


        int fp = lseek(fd, 0, SEEK_END);

        if (fp == 0) {
                acc.id = 1;
        } 
        else {
                fp = lseek(fd, -1 * acc_size, SEEK_CUR);
                read(fd, & acc, sizeof(acc));
                acc.id++;
        }
        strcpy(acc.name, name);
        strcpy(acc.pass, password);
        write(fd, & acc, sizeof(acc));
        write(sd, & acc.id, sizeof(acc.id));

  
        close(fd);
        return 3;
}

int Adminmenu(int sd, int id) {
        int ch;
        read(sd, & ch, sizeof(int));
        if (ch == 1) {
                //---------------------------------------------------------------add a train---------
                int tid = 0;
                int tno;
                char tname[20];
                read(sd, & tname, sizeof(tname));
                read(sd, & tno, sizeof(int));
                struct train temp, temp2;

                temp.tid = tid;
                temp.train_no = tno;
                strcpy(temp.train_name, tname);
                temp.seats_avl = 20;
                temp.booked_seat = 0;

                int fd = open("db/train", O_RDWR);


                int fp = lseek(fd, 0, SEEK_END);
                if (fp == 0) {
                        write(fd, & temp, sizeof(temp));
                        write(sd, & ch, sizeof(int));

                        close(fd);
                } else {
                        lseek(fd, -1 * trn_size, SEEK_CUR);
                        read(fd, & temp2, sizeof(temp2));
                        temp.tid = temp2.tid + 1;
                        write(fd, & temp, sizeof(temp));
                        write(sd, & ch, sizeof(int));

                        close(fd);
                }
                return ch;
        }
        if (ch == 2) {
                        //--------------------------------------------------------------------------delete a train-------
                int fd = open("db/train", O_RDWR);

                struct flock lock;
                lock.l_type = F_WRLCK;
                lock.l_start = 0;
                lock.l_len = 0;
                lock.l_whence = SEEK_SET;
                lock.l_pid = getpid();

                fcntl(fd, F_SETLKW, & lock);

                int fp = lseek(fd, 0, SEEK_END);
                int train_cnt = fp / trn_size;
                int train_id;
                write(sd, & train_cnt, sizeof(int));
                lseek(fd, 0, SEEK_SET);
                while (fp != lseek(fd, 0, SEEK_CUR)) {
                        struct train temp;
                        read(fd, & temp, trn_size);
                        write(sd, & temp.tid, sizeof(int));
                        write(sd, & temp.train_name, sizeof(temp.train_name));
                        write(sd, & temp.train_no, sizeof(int));
                }
                read(sd, & train_id, sizeof(int));
                if (train_id <= 0 || train_id > train_cnt) {
                        train_id = 0; //to abort operation
                        write(sd, & train_id, sizeof(int));
                } 
                else {
                        struct train temp;
                        lseek(fd, 0, SEEK_SET);
                        lseek(fd, (train_id - 1) * trn_size, SEEK_CUR);
                        read(fd, & temp, trn_size);
                        printf("%s is deleted\n", temp.train_name);
                        strcpy(temp.train_name, "deleted");
                        lseek(fd, -1 * trn_size, SEEK_CUR);
                        write(fd, & temp, trn_size);
                        write(sd, & train_id, sizeof(int));
                }

                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, & lock);
                close(fd);
        }
        if (ch == 3) {//--------------------------------------Modify Train---------------------------------------
                int fd = open("db/train", O_RDWR);

                struct flock lock;
                lock.l_type = F_WRLCK;
                lock.l_start = 0;
                lock.l_len = 0;
                lock.l_whence = SEEK_SET;
                lock.l_pid = getpid();

                fcntl(fd, F_SETLKW, & lock);

                int fp = lseek(fd, 0, SEEK_END);
                int train_cnt = fp / trn_size;
                int train_id;
                write(sd, & train_cnt, sizeof(int));
                lseek(fd, 0, SEEK_SET);
                while (fp != lseek(fd, 0, SEEK_CUR)) {
                        struct train temp;
                        read(fd, & temp, trn_size);
                        write(sd, & temp.tid, sizeof(int));
                        write(sd, & temp.train_name, sizeof(temp.train_name));
                        write(sd, & temp.train_no, sizeof(int));
                }
                read(sd, & train_id, sizeof(int)); //0: cancel, others : train_ID

                if (train_id <= 0 || train_id > train_cnt) {
                        train_id = 0;
                        write(sd, & train_id, sizeof(train_id)); //not valid and abort operation
                        lock.l_type = F_UNLCK;
                        fcntl(fd, F_SETLK, & lock);
                        close(fd);
                        return ch;
                } else {
                        write(sd, & train_id, sizeof(train_id)); //valid operation
                        int choice;
                        struct train temp;
                        lseek(fd, 0, SEEK_SET);
                        lseek(fd, (train_id - 1) * trn_size, SEEK_CUR);
                        read(fd, & temp, trn_size);

                        read(sd, & choice, sizeof(int)); //1: name,2: train no, 3: seats
                        if (choice == 1) {
                                char name[20];
                                write(sd, & temp.train_name, sizeof(temp.train_name));
                                read(sd, & name, sizeof(name));
                                strcpy(temp.train_name, name);
                        } else if (choice == 2) {

                                write(sd, & temp.seats_avl, sizeof(temp.seats_avl));
                                read(sd, & temp.seats_avl, sizeof(temp.seats_avl));
                        }

                        printf("%s\t%d\t%d\n", temp.train_name, temp.train_no, temp.seats_avl);
                        lseek(fd, -1 * trn_size, SEEK_CUR);
                        write(fd, & temp, trn_size);
                        write(sd, & choice, sizeof(int));

                        lock.l_type = F_UNLCK;
                        fcntl(fd, F_SETLK, & lock);
                        close(fd);
                        return ch;
                }
        }
        if (ch == 5) {

                return 0;
        }
        if (ch == 6) {//--------------------delete user---------------------------
                int type, id;
                struct account
                var;
                read(sd, & type, sizeof(int));

                int fd = open(ACCOUNT[type - 1], O_RDWR);
                struct flock lock;
                lock.l_type = F_WRLCK;
                lock.l_start = 0;
                lock.l_whence = SEEK_SET;
                lock.l_len = 0;
                lock.l_pid = getpid();

                fcntl(fd, F_SETLKW, & lock);

                int fp = lseek(fd, 0, SEEK_END);
                int users = fp / acc_size;
                write(sd, & users, sizeof(int));

                lseek(fd, 0, SEEK_SET);
                while (fp != lseek(fd, 0, SEEK_CUR)) {
                        read(fd, &var, acc_size);
                        write(sd, &var.id, sizeof(var.id));
                        write(sd, &var.name, sizeof(var.name));
                }

                read(sd, & id, sizeof(int));
                if (id == 0 || id > users) {
                        id = 0;
                        write(sd, & id, sizeof(int));
                } else {
                        lseek(fd, 0, SEEK_SET);
                        lseek(fd, (id - 1) * acc_size, SEEK_CUR);
                        read(fd, &var, acc_size);
                        lseek(fd, -1 * acc_size, SEEK_CUR);
                        strcpy(var.name, "deleted");
                        strcpy(var.pass, "");
                        write(fd, &var, acc_size);
                        write(sd, & ch, sizeof(int));
                }

                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, & lock);

                close(fd);

                return ch;
        }

        if (ch == 8) {
                write(sd, & ch, sizeof(int));
                return -1;
        }

}

int Usermenu(int sd, int id, int type) {
        int ch;
        read(sd, & ch, sizeof(int));
        if (ch == 1) {
                //-----------------------------------------------------------book a ticket--------------------
                int fd = open("db/train", O_RDWR);
                int t_cnt;
                struct flock lock;
                lock.l_type = F_WRLCK;
                lock.l_start = 0;
                lock.l_len = 0;
                lock.l_whence = SEEK_SET;
                lock.l_pid = getpid();

                fcntl(fd, F_SETLKW, & lock);

                struct train temp;
                int fp = lseek(fd, 0, SEEK_END);
                int train_cnt = fp / trn_size;

                struct train av_trains[train_cnt];
                write(sd, & train_cnt, sizeof(int));
                printf("No of trains : %d\n", train_cnt);
                lseek(fd, 0, SEEK_SET);
                t_cnt = 0;
                while (fp != lseek(fd, 0, SEEK_CUR)) {
                        read(fd, & temp, trn_size);
                        av_trains[t_cnt++] = temp;
                        write(sd, &temp.tid, sizeof(int));
                        write(sd, &temp.train_no, sizeof(int));
                        write(sd, &temp.seats_avl, sizeof(int));
                        write(sd, &temp.train_name, sizeof(temp.train_name));
                        write(sd, &temp.booked_seat, sizeof(temp.booked_seat));
                }

                int trainid, seats;
                read(sd, &trainid, sizeof(int));

                int v = 0;
                char * del = "deleted";
                for (int i = 0; i < t_cnt; i++) {
                        if (av_trains[i].tid + 1 == trainid) {
                                printf("%d %s \n", av_trains[i].tid, av_trains[i].train_name);
                                if (strcmp(av_trains[i].train_name, del) == 0)
                                        v = 0;
                                else
                                        v = 1;
                                break;
                        }
                }

                write(sd, & v, sizeof(v));
                if (v) {

                        lseek(fd, 0, SEEK_SET);
                        lseek(fd, (trainid - 1) * trn_size, SEEK_CUR);
                        read(fd, & temp, trn_size);
                        write(sd, & temp.seats_avl, sizeof(int));
                        printf("Train ID: %d, fetched ID: %d\n", trainid, temp.tid);
                        read(sd, & seats, sizeof(int));
                        if (seats > 0) {
                                temp.seats_avl -= seats;
                                int fd2 = open("db/booking", O_RDWR);
                                fcntl(fd2, F_SETLKW, & lock);
                                struct bookings bk;
                                int fp2 = lseek(fd2, 0, SEEK_END);
                                if (fp2 > 0) {
                                        lseek(fd2, -1 * book_size, SEEK_CUR);
                                        read(fd2, & bk, book_size);
                                        bk.bid++;
                                } else
                                        bk.bid = 0;
                                bk.type = type;
                                bk.acc_no = id;
                                bk.tr_id = trainid;
                                bk.cancelled = 0;
                                strcpy(bk.trainname, temp.train_name);
                                bk.seat_start = temp.booked_seat + 1;
                                bk.seat_end = temp.booked_seat + seats;
                                temp.booked_seat = bk.seat_end;
                                write(fd2, & bk, sizeof(bk));
                                lock.l_type = F_UNLCK;
                                fcntl(fd2, F_SETLK, & lock);
                                close(fd2);
                        }
                        lseek(fd, -1 * trn_size, SEEK_CUR);
                        write(fd, & temp, sizeof(temp));
                        if (seats <= 0)
                                ch = -1;
                        write(sd, & ch, sizeof(int));
                }
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, & lock);
                close(fd);
                return 1;
        }

        if (ch == 2) {//------------------------------------------view previous bookings-------------------
                View_Bookings(sd, id, type);
                write(sd, & ch, sizeof(int));
                return 2;
        }
        if (ch == 3) {
                //---------------------------------------------------update booking------------------------
                View_Bookings(sd, id, type);

                int fd1 = open("db/train", O_RDWR);
                int fd2 = open("db/booking", O_RDWR);
                struct flock lock;
                lock.l_type = F_WRLCK;
                lock.l_start = 0;
                lock.l_len = 0;
                lock.l_whence = SEEK_SET;
                lock.l_pid = getpid();

                fcntl(fd1, F_SETLKW, & lock);
                fcntl(fd2, F_SETLKW, & lock);

                int val;
                struct train temp1;
                struct bookings temp2;
                read(sd, & val, sizeof(int)); //Read the Booking ID to updated

                // read the booking to be updated
                lseek(fd2, 0, SEEK_SET);
                lseek(fd2, val * book_size, SEEK_CUR);
                read(fd2, & temp2, sizeof(temp2));
                lseek(fd2, -1 * book_size, SEEK_CUR);
                printf("%d %s %d\n", temp2.tr_id, temp2.trainname, temp2.seat_end);

                // read the train details of the booking
                lseek(fd1, 0, SEEK_SET);
                lseek(fd1, (temp2.tr_id - 1) * trn_size, SEEK_CUR);
                read(fd1, & temp1, sizeof(temp1));
                lseek(fd1, -1 * trn_size, SEEK_CUR);
                printf("%d %s %d\n", temp1.tid, temp1.train_name, temp1.seats_avl);

                read(sd, & val, sizeof(int)); //Increase or Decrease

                if (val == 1) { //increase
                        read(sd, & val, sizeof(int)); //No of Seats
                        if (temp1.seats_avl >= val) {
                                temp2.cancelled = 1;
                                temp1.seats_avl += val;
                                write(fd2, & temp2, sizeof(temp2));

                                int total = temp2.seat_end - temp2.seat_start + 1 + val;
                                struct bookings bk;

                                int fp2 = lseek(fd2, 0, SEEK_END);
                                lseek(fd2, -1 * book_size, SEEK_CUR);
                                read(fd2, & bk, book_size);

                                bk.bid++;
                                bk.type = temp2.type;
                                bk.acc_no = temp2.acc_no;
                                bk.tr_id = temp2.tr_id;
                                bk.cancelled = 0;
                                strcpy(bk.trainname, temp2.trainname);
                                bk.seat_start = temp1.booked_seat + 1;
                                bk.seat_end = temp1.booked_seat + total;

                                temp1.seats_avl -= total;
                                temp1.booked_seat = bk.seat_end;

                                write(fd2, & bk, sizeof(bk));
                                write(fd1, & temp1, sizeof(temp1));
                        } else {
                                ch = -2;
                                write(sd, & ch, sizeof(int));
                        }
                } else { //decrease			
                        read(sd, & val, sizeof(int)); //No of Seats
                        if (temp2.seat_end - val < temp2.seat_start) {
                                temp2.cancelled = 1;
                                temp1.seats_avl += val;
                        } else {
                                temp2.seat_end -= val;
                                temp1.seats_avl += val;
                        }
                        write(fd2, & temp2, sizeof(temp2));
                        write(fd1, & temp1, sizeof(temp1));
                }
                lock.l_type = F_UNLCK;
                fcntl(fd1, F_SETLK, & lock);
                fcntl(fd2, F_SETLK, & lock);
                close(fd1);
                close(fd2);
                if (ch > 0)
                        write(sd, & ch, sizeof(int));
                return 3;

        }
        if (ch == 4) {
                //---------------------------------------------------------Cancel booking-------------------------------------
    		View_Bookings(sd, id, type);
		int bid;
		struct bookings booking;
		struct train tr;
		read(sd,&bid,sizeof(bid));

		int fd = open("db/booking", O_RDWR);
		struct flock lock,lock2;
		lock.l_type = F_WRLCK;
		lock.l_start = 0;
		lock.l_len = 0;
		lock.l_whence = SEEK_SET;
		lock.l_pid = getpid();
		
		fcntl(fd, F_SETLKW, &lock);

		lseek(fd,(bid-1)*sizeof(struct bookings),SEEK_SET);

		read(fd,&booking,sizeof(struct bookings));
		
		if(booking.cancelled == 0 && booking.acc_no == id && booking.type == type){


			booking.cancelled = 1;
			int fd2 = open("db/train",O_RDWR);
			
			fcntl(fd2,F_SETLKW,&lock);
			lseek(fd2,(booking.tr_id -1)*sizeof(struct train),SEEK_SET);
			read(fd2,&tr,sizeof(struct train));
			tr.seats_avl += (booking.seat_end-booking.seat_start+1);
			lseek(fd2,-1*sizeof(struct train),SEEK_CUR);
			write(fd2,&tr,sizeof(struct train));

			lock.l_type = F_UNLCK;
			fcntl(fd2,F_SETLK,&lock);
			close(fd2);
			write(sd, &ch, sizeof(ch));
		
		}
		else{
			ch = 0;
			write(sd, &ch, sizeof(ch));
		}
		lseek(fd,-1 * sizeof(struct bookings),SEEK_CUR);
		write(fd,&booking,sizeof(struct bookings));
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		close(fd);

		return 4;
        }
        if (ch == 5) {
                write(sd, & ch, sizeof(int));
                return -1;
        }
        return 0;
}

void View_Bookings(int sd, int id, int type) {
        int fd = open("db/booking", O_RDONLY);

        struct flock lck;                      //--------mandatory locking is applied while user is viewing the booking 
        lck.l_type = F_RDLCK;
        lck.l_start = 0;
        lck.l_len = 0;
        lck.l_whence = SEEK_SET;
        lck.l_pid = getpid();
        fcntl(fd, F_SETLKW, & lck);

        int fp = lseek(fd, 0, SEEK_END);
        int ent = 0;
        if (fp == 0)
                write(sd, & ent, sizeof(ent));
        else {
                struct bookings bk[10];
                while (fp > 0 && ent < 10) {
                        struct bookings temp;
                        fp = lseek(fd, -1 * book_size, SEEK_CUR);
                        read(fd, & temp, book_size);
                        if (temp.acc_no == id && temp.type == type)
                                bk[ent++] = temp;
                        fp = lseek(fd, -1 * book_size, SEEK_CUR);
                }
                write(sd, & ent, sizeof(int));
                fp = 0;
                while (fp < ent) {
                        write(sd, & bk[fp].bid, sizeof(bk[fp].bid));
                        write(sd, & bk[fp].tr_id, sizeof(bk[fp].tr_id));
                        write(sd, & bk[fp].seat_start, sizeof(int));
                        write(sd, & bk[fp].seat_end, sizeof(int));
                        write(sd, & bk[fp].cancelled, sizeof(int));
                        fp++;
                }
        }
        lck.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, & lck);
        close(fd);
}

void mainmenu(int sd) {
        int ch;

        while (1) {
                read(sd, & ch, sizeof(int));
                if (ch == 1) login(sd);
                else
                if (ch == 2) signup(sd);
                else
                        break;
        }
        close(sd);

}
