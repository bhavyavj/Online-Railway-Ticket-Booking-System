int  mainmenu(int sd);
int  Display_bar(int sd, int t);
void View_Bookings(int sd);
int  Adminmenu(int sd, int ch);
int  Usersmenu(int sd, int ch); // customer and agent

int mainmenu(int sd) {
        int ch;

        printf("\t\t*Welcome to Online Railways Ticket Booking System*\n\n");
        printf("1. Login\n");
        printf("2. Sign Up\n");
        printf("3. Exit\n");
        printf("Enter your choice\n");
        scanf("%d", & ch);
        write(sd, & ch, sizeof(ch));

        //-------------------------Login----------------------
        if (ch == 1) {
                int type, acc_no;
                char password[20], secret_pin[6];
                printf("Enter the type of account:\n");
                printf(" 1.Customer\n 2.Agent\n 3.Admin\n 4.Back\n ");
                printf("Enter your choice: ");
                scanf("%d", & type);

                if (type == 4) //-------------------Back to mainmenu---------------
                {

                        write(sd, & type, sizeof(type));
                        return 0;
                }

                printf("Enter Account No.: ");
                scanf("%d", & acc_no);
                strcpy(password, getpass("Enter password: "));
                if (type == 3) {
                        while (1) {
                                strcpy(secret_pin, getpass("Enter PIN to access the ADMIN account: "));
                                if (strcmp(secret_pin, "1111") != 0)
                                        printf("Invalid PIN. Retry....:\n");
                                else
                                        break;
                        }
                }

                write(sd, & type, sizeof(type));
                write(sd, & acc_no, sizeof(acc_no));
                write(sd, & password, strlen(password));

                int valid_login;
                read(sd, & valid_login, sizeof(valid_login));
                if (valid_login == 1) {
                        while (Display_bar(sd, type) != -1); // for valid user it leads to their menu

                        return 1;
                } else {
                        printf("Login Failed\n");
                        while (getchar() != '\n');
                        getchar();
                        return 1;
                }
        } else
                //-----------------------------------Sign up-------------------------
                if (ch == 2) {
                        int type, acc_no;
                        char password[20], name[10], secret_pin[6];
                        printf("Enter the type of account :\n");
                        printf("1.Customer\n2.Agent\n3.Admin\n4.Back\n");
                        printf("Your Choice: ");
                        scanf("%d", & type);

                        if (type == 4) //-------------------Back to mainmenu---------------
                        {
                                write(sd, & type, sizeof(type));
                                return 0;
                        }

                        printf("Enter your name : ");
                        scanf("%s", name);
                        strcpy(password, getpass("Enter password: "));

                        if (type == 3) {
                                while (1) {
                                        strcpy(secret_pin, getpass("Enter PIN to create ADMIN account: "));
                                        if (strcmp(secret_pin, "1111") != 0)
                                                printf("Invalid PIN. Please Try Again.\n");
                                        else
                                                break;
                                }
                        }
                        write(sd, & type, sizeof(type));
                        write(sd, & name, sizeof(name));
                        write(sd, & password, strlen(password));

                        read(sd, & acc_no, sizeof(acc_no));
                        printf("Your account No. is : %d.\nThe account no of further login\n", acc_no);
                        while (getchar() != '\n');
                        getchar();
                        return 2;
                }
        else
                //----------------------Exit---------------------------------- 
                if (ch == 3)
                        return 3;

}

int Display_bar(int sd, int type) {
        int choice_ = 0;
        //-------------------------------Menu for Customers and Agent------------
        if (type == 1 || type == 2) {

                printf("1. Book Ticket\n2. View Previous Bookings\n3. Update Booking\n4. Cancel Booking\n5. Logout\n");
                printf("Your Choice: ");
                scanf("%d", & choice_);
                return Usersmenu(sd, choice_);
                return -1;
        } else {
                //-------------------------------Menu for Admin----------------------

                printf("1. Add Train\n2. Delete Train\n3. Modify Train\n4. Display trains\n5. Display Users \n6. Delete User\n7. View Booking\n8. Logout\n");
                printf("Your Choice: ");
                scanf("%d", & choice_);
                return Adminmenu(sd, choice_);
        }
}

int Adminmenu(int sd, int choice_) {
        int fd, fp;
        switch (choice_) {
                //----------------------------------Train add by admin-----------------------------------
        case 1: {
                int tno;
                char tname[20];
                write(sd, & choice_, sizeof(choice_));

                printf("Enter Train Name: ");
                scanf("%s", tname);
                printf("Enter Train No. : ");
                scanf("%d", & tno);

                write(sd, & tname, sizeof(tname));
                write(sd, & tno, sizeof(tno));
                read(sd, & choice_, sizeof(choice_));
                if (choice_ == 1) printf("Train Added Successfully.\n");
                while (getchar() != '\n');
                getchar();
                return choice_;
                break;
        }

        //-------------------------------Train deletion------------------------
        case 2: {
                int train_cnt;
                write(sd, & choice_, sizeof(choice_));
                read(sd, & train_cnt, sizeof(int));
                while (train_cnt > 0) { //print the train data of the train which has not been deleted yet
                        int tid, tno;
                        char tname[20];
                        read(sd, & tid, sizeof(tid));
                        read(sd, & tname, sizeof(tname));
                        read(sd, & tno, sizeof(tno));
                        if (!strcmp(tname, "deleted"));
                        else
                                printf("%d.\t%d\t%s\n", tid + 1, tno, tname);
                        train_cnt--;
                }
                printf("Enter 0 to cancel.\nEnter the train ID to delete: ");
                scanf("%d", & train_cnt);
                write(sd, & train_cnt, sizeof(int));
                read(sd, & choice_, sizeof(choice_));

                if (choice_ == 0)
                        printf("Deletion Cancelled"); //cancel 
                else printf("Train with train_id %d has been deleted\n", choice_);

                while (getchar() != '\n');
                getchar();
                return choice_;
                break;
        }
        //-------------------------------Modify train data --------------------------------
        case 3: {
                int train_cnt, is_valid;
                write(sd, & choice_, sizeof(choice_));
                read(sd, & train_cnt, sizeof(int));
                while (train_cnt > 0) {
                        int tid, tno;
                        char tname[20];
                        read(sd, & tid, sizeof(tid));
                        read(sd, & tname, sizeof(tname));
                        read(sd, & tno, sizeof(tno));
                        if (!strcmp(tname, "deleted"));
                        else
                                printf("%d.\t%d\t%s\n", tid + 1, tno, tname);
                        train_cnt--;
                }
                printf("Enter 0 to cancel.\nEnter the train ID to modify: ");
                scanf("%d", & train_cnt);
                write(sd, & train_cnt, sizeof(int));
                read(sd, & is_valid, sizeof(is_valid));
                if (is_valid == 0) {
                        printf("Delection Cancelled!\n");
                } else
                if (is_valid != 0) {
                        printf("Enter parameter do you want to modify:\n1. Train Name\n2. Available Seats\n Enter Choice: ");
                        //printf("");
                        scanf("%d", & train_cnt);
                        write(sd, & train_cnt, sizeof(int));
                        // if(train_cnt == 2 || train_cnt == 3){
                        if (train_cnt == 2) {

                                read(sd, & train_cnt, sizeof(int));
                                printf("Current Value: %d\n", train_cnt);
                                printf("Enter Value: ");
                                scanf("%d", & train_cnt);
                                write(sd, & train_cnt, sizeof(int));
                        } else if (train_cnt == 1) {
                                char name[20];
                                read(sd, & name, sizeof(name));
                                printf("Current Value: %s\n", name);
                                printf("Enter Value: ");
                                scanf("%s", name);
                                write(sd, & name, sizeof(name));
                        }
                        read(sd, & choice_, sizeof(choice_));
                        if (choice_ == train_cnt) printf("Train Data Modified Successfully\n"); //if modified
                }
                while (getchar() != '\n');
                getchar();
                return choice_;
                break;
        }
        case 4: {
                //-------------------------------Display trains-----------
                fd = open("db/train", O_RDONLY);
                fp = lseek(fd, 0, SEEK_SET);
                struct train tr;
                printf("ID\tT_NO\tSEAT_AVAIL\tTRAIN NAME\tLast_Booked_Seatno\n");
                while (read(fd, & tr, sizeof(tr)))
                        printf("%d\t%d\t%d\t%s\t\t%d\n", tr.tid, tr.train_no, tr.seats_avl, tr.train_name, tr.booked_seat);
                close(fd);
                while (getchar() != '\n');
                getchar();
                return choice_;
                break;
        }

        case 5: //-------------------------display users--------
        {
                printf("1. Customer\n");
                printf("2. Agents\n");
                printf("3. Admins\n");
                printf("Enter Your Choice\n");
                int opt2;
                struct account acc;
                scanf("%d", & opt2);
                printf("ID\t\tName\n");
                switch (opt2) {
                case 1:
                        fd = open("db/accounts/customer", O_RDONLY);
                        fp = lseek(fd, 0, SEEK_SET);
                        while (read(fd, & acc, sizeof(acc)))
                                printf("%d\t\t%s\n", acc.id, acc.name);
                        close(fd);
                        break;
                case 2:
                        fd = open("db/accounts/agent", O_RDONLY);
                        fp = lseek(fd, 0, SEEK_SET);
                        while (read(fd, & acc, sizeof(acc)))
                                printf("%d\t\t%s\n", acc.id, acc.name);
                        close(fd);
                        break;
                case 3:
                        fd = open("db/accounts/admin", O_RDONLY);
                        fp = lseek(fd, 0, SEEK_SET);
                        while (read(fd, & acc, sizeof(acc)))
                                printf("%d\t\t%s\n", acc.id, acc.name);
                        close(fd);
                        break;
                default:
                        printf("Invalid Choice\n");
                        break;
                }
                while (getchar() != '\n');
                getchar();
                return choice_;
                break;
        }
        case 6: {
                int choice, users, id;
                write(sd, & choice_, sizeof(choice_));
                printf("Enter the type of account you want to delete?\n");
                printf("1. Customer\n2. Agent\n3. Admin\n");
                printf("Your Choice: ");
                scanf("%d", & choice);
                while (choice < 1 || choice > 3);
                write(sd, & choice, sizeof(choice));
                read(sd, & users, sizeof(users));
                while (users--) {
                        char name[10];
                        read(sd, & id, sizeof(id));
                        read(sd, & name, sizeof(name));
                        if (strcmp(name, "deleted") != 0)
                                printf("%d\t%s\n", id, name);
                }
                printf("Enter 0 to cancel\nEnter the ID to delete: ");
                scanf("%d", & id);
                write(sd, & id, sizeof(id));
                read(sd, & choice_, sizeof(choice_));
                if (choice_ == 0) {
                        printf("Deletion Cancelled\n");
                } else {
                        printf("Successfully deleted user\n");
                }
                while (getchar() != '\n');
                getchar();
                return choice_;
        }
        case 7: {
                //---------------------------------view bookings------

                fd = open("db/booking", O_RDONLY);
                fp = lseek(fd, 0, SEEK_SET);
                struct bookings book;
                printf("BookingID\tUserID\tType\t1st Ticket\tLast Ticket\tTrainId\tTrainName\tisCancelled\n");
                while (read(fd, & book, sizeof(book))) printf("%d\t\t%d\t%d\t%d\t\t%d\t\t%d\t%s\t\t%d\n", book.bid, book.acc_no, book.type, book.seat_start, book.seat_end, book.tr_id, book.trainname, book.cancelled);
                close(fd);
                while (getchar() != '\n');
                getchar();
                return choice_;
                break;
        }

        case 8: { //---------------------------------------------log out-----------------
                write(sd, & choice_, sizeof(choice_));
                read(sd, & choice_, sizeof(choice_));
                if (choice_ == 8) printf("Admin Logged out \n");
                while (getchar() != '\n');
                getchar();
                return -1;
                break;
        }

        default:
                return -1;
        }
}

int Usersmenu(int sd, int choice_) {
        write(sd, & choice_, sizeof(choice_));
        switch (choice_) {
        case 1: {
                //-------------------------------------------book a ticket-------------------------------
                int trains, trainid, trainavseats, trainno, required_seats, last_booked_seat, lastTrainId, valid = 0;
                int t_cnt;
                char trainname[20];
                read(sd, & trains, sizeof(trains));
                // struct train av_trains[trains];
                printf("ID\tT_NO\tAV_SEAT\tTRAIN NAME\n");

                // t_cnt=0;
                while (trains--) {
                        read(sd, & trainid, sizeof(int));
                        read(sd, & trainno, sizeof(int));
                        read(sd, & trainavseats, sizeof(int));
                        read(sd, & trainname, sizeof(trainname));
                        read(sd, & last_booked_seat, sizeof(int));

                        if (strcmp(trainname, "deleted") != 0) {
                                printf("%d\t%d\t%d\t%s\n", trainid + 1, trainno, trainavseats, trainname);
                        }
                }
                lastTrainId = trainid;
                printf("Enter the train ID: ");
                scanf("%d", & trainid);

                write(sd, & trainid, sizeof(trainid));
                read(sd, & valid, sizeof(valid));

                if (valid) {

                        read(sd, & trainavseats, sizeof(trainavseats));
                        printf("Available seats : %d\n", trainavseats);
                        printf("Enter the number of seats: ");
                        scanf("%d", & required_seats);
                        if (trainavseats >= required_seats && required_seats > 0)
                                write(sd, & required_seats, sizeof(required_seats));
                        else {
                                required_seats = -1;
                                write(sd, & required_seats, sizeof(required_seats));
                        }
                        read(sd, & choice_, sizeof(choice_));

                        if (choice_ == 1) printf("Tickets booked successfully\n");
                        else printf("Tickets were not booked. Please try again.\n");
                } else {
                        printf("Operation Aborded\n");
                }
               
                while (getchar() != '\n');
                getchar();
                while (!getchar());
                return 1;
        }break;
                
        case 2: { //----------------------------------------------------------View your bookings---------------------
                View_Bookings(sd);
                read(sd, & choice_, sizeof(choice_));
                return 2;
        }break;
                
        case 3: { //------------------------------------------------------------Update bookings-----------------------
                int val;
                View_Bookings(sd);
                printf("Enter the booking id to be updated: ");
                scanf("%d", & val);
                write(sd, & val, sizeof(int)); //Booking ID
                printf("What information do you want to update:\n1.Increase No of Seats\n2. Decrease No of Seats\nYour Choice: ");
                scanf("%d", & val);
                write(sd, & val, sizeof(int)); //Increase or Decrease
                if (val == 1) {
                        printf("Enter no. of tickets you want to increase: ");
                        scanf("%d", & val);
                        write(sd, & val, sizeof(int)); //No of Seats
                } else
                if (val == 2) {
                        printf("Enter no. of tickets you want to decrease: ");
                        scanf("%d", & val);
                        write(sd, & val, sizeof(int)); //No of Seats		
                }
                read(sd, & choice_, sizeof(choice_));
                if (choice_ == -2)
                        printf("Operation failed. No more available seats\n");
                else printf("Operation succeded.\n");
                while (getchar() != '\n');
                getchar();
                return 3;
        }break;
                
        case 4: {
                //--------------------------------cancel booking-----------------------------------
     
                int bid;
			View_Bookings(sd);
			printf("Enter the booking id to be Cancelled: "); scanf("%d", &bid);
			write(sd, &bid, sizeof(int));	//Booking ID
			read(sd,&choice_,sizeof(int));
			if(choice_){
				printf("Your Booking has been Cancelled Succesfully\n");
			}
			else{
				printf("Cancellation failed!\n");
			}
			getchar();
			getchar();
			return 4;  }break;
                
                
        case 5: { //----------------------Logout------------------------
                read(sd, & choice_, sizeof(choice_));
                if (choice_ == 5) printf("User Logged out\n");
                while (getchar() != '\n');
                getchar();
                return -1;
                break;	}break;
                
                
        default:return -1;break;
                
        }
}

void View_Bookings(int sd) {
        int entries,c=0,t;
        read(sd, & entries, sizeof(int));
        
        if (entries == 0) printf("No booking has been done so far...\n");
       else t=entries;//printf("Your recent bookings are mentioned below\n");
        while (!getchar());
        printf("BookingID \tTrainNo. \tNo_of_seats \n");
        while (entries--) {
                int bid, bks, bke, cancelled, tno;
                read(sd, & bid, sizeof(bid));
                read(sd, & tno, sizeof(int));
                read(sd, & bks, sizeof(int));
                read(sd, & bke, sizeof(int));
                read(sd, & cancelled, sizeof(int));
                if (!cancelled)
                        printf(" %d\t\t %d\t \t%d\n", bid + 1, tno, bke - bks + 1);
                        else
                        c++;
        }
        if (t == c) printf("Sorry!! All recent bookings are showing cancelled status.\n");
        while (getchar() != '\n');
        getchar();
}
