struct account
{
	int id;
	char name[10];
	char pass[20];
};

struct bookings
{
	int bid;
	int type;
	int acc_no;
	int tr_id;
	char trainname[20];
	int seat_start;
	int seat_end;
	int cancelled;
};

struct train
{
	int tid;
	char train_name[20];
	int train_no;
	int seats_avl;
	int booked_seat;
};

