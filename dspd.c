# include <stdio.h>
# include <string.h>
# include <time.h>
# include <stdlib.h>
#define _XOPEN_SOURCE 700
# define NUM_LEN 20
# define NAME_LEN 50
# define TIME_LEN 50
# define STATUS_LEN 10
# define SPACE_DB_LEN 50
# define VEHICLE_DB_LEN 100

struct vehicle_details
{
     char vehicle_number[NUM_LEN];
     char owner_name[NUM_LEN];
     time_t arrival_time;
     time_t departure_time;
     char membership[STATUS_LEN];
     long int total_parking_hours;
     int parking_space_ID;
     int parking_count;
     double total_amount_paid;
};
struct parking_space_status
{
     int parking_space_ID;
     int status;
     int occupancy_count;
     double revenue_generated;
};
//Setting default values in every field of structure
void initialize_vehicle_DB(struct vehicle_details arr[])
{
     int j;
     for(j=0;j<VEHICLE_DB_LEN;j++)
     {
          arr[j].vehicle_number[0]='\0';
          arr[j].owner_name[0]='\0';
          strcpy(arr[j].membership,"NULL");
          arr[j].total_parking_hours=0;
          arr[j].parking_space_ID=0;
          arr[j].parking_count=0;
          arr[j].total_amount_paid=0;
     }
}  
void initialize_parking_spaces(struct parking_space_status arr[])
{
     int k;
     for(k=0;k<SPACE_DB_LEN;k++)
     {
         arr[k].parking_space_ID=k+1;
         arr[k].status=0;
         arr[k].occupancy_count=0;
         arr[k].revenue_generated=0;
     }
}
//function to check if vehicle is registered or not ,if registered returns its index and if not it returns -1
int registration_index(struct vehicle_details arr[],char vehicle_num[],int entry_count)
{
     int index=-1,i;
     for(i=0;i<entry_count && index==-1 ;i++)
     {
          if(strcmp(vehicle_num,arr[i].vehicle_number)==0)
          {
               index=i;
          }
     }
    return index;
}
//function to search a free parking space and written in position
int search_for_space(struct vehicle_details arr[],struct parking_space_status brr[],char vehicle_num[],int entry_count)
{
     int index=registration_index(arr,vehicle_num,entry_count);
     int free_loc=-1;
         //if vehicle is registered and have golden membership
          if(index >-1 && strcmp(arr[index].membership,"GOLDEN")==0)
          {
                 int i=0;
                 while(i<50 && free_loc==-1)
                 {
                      if(brr[i].status==0)
                      {
                           free_loc=i;
                      }
                      i++;
                  }                                                          
          }
          else if(index >-1 && strcmp(arr[index].membership ,"PREMIUM")==0)//vehicle have premium membership
          {
                      int j=10;
                      while(j<50 && free_loc==-1)
                      {
                           if(brr[j].status==0)
                           {
                                 free_loc=j;
                           }
                           j++;
                      }
          }
          else //vehicle have no membership and it is registered or not registered
          {
           
                 int k=20;
                 while(k<SPACE_DB_LEN && free_loc==-1)
                 {
                        if(brr[k].status==0)
                        {
                            free_loc=k;
                        }
                        else
                        k++;
                 }
           }
return free_loc;
}
// Function to insert details for a new vehicle entry and update details for a registered vehicle entry
void Update_Insert_vehicle_details(struct vehicle_details arr[], struct parking_space_status brr[], char vehicle_num[], char name[], time_t time, int *entry_count)
 {
    int entries = *entry_count;
    int space = search_for_space(arr, brr, vehicle_num, entries);
    
    if (space < 0)
    {
        printf("Parking space is exhausted, cannot insert or update your details.\n");
    }
    else
    {
        // Checking if vehicle is already registered or not
        int index = registration_index(arr, vehicle_num, entries);
       	if (index < 0)
	     { // Vehicle is not registered
            if (*entry_count < 100)
	         { // Check if the entry limit has been reached
               	// Insert the information for a new vehicle entry
                index = *entry_count; // Use current count as the index
                strcpy(arr[index].vehicle_number, vehicle_num);
                strcpy(arr[index].owner_name, name);
                arr[index].arrival_time = time;
                arr[index].parking_count = 1;
                *entry_count = *entry_count + 1;
                brr[space].occupancy_count++;
                brr[space].status = 1;

                printf("Vehicle registered successfully\nParking space ID: %d\n", space + 1);
             }
	         else
	         {
              // Vehicle details database exhausted
              printf("Vehicle details entry limit is exhausted, cannot register new user.\n");
              }
         }
	     else
	     {  
            arr[index].arrival_time = time;
            arr[index].parking_count++;
            brr[space].status = 1;
            brr[space].occupancy_count++;

            printf("Information updated successfully\nParking space ID: %d\n", space + 1);
         }
         
    }
}

void exit_updation(struct vehicle_details arr[], struct parking_space_status brr[], int ID, char vehicle_num[], time_t departure_time, int entry_count)
{
    int space = ID - 1;
    int k = registration_index(arr, vehicle_num, entry_count); // Use the vehicle number entered during exit
    

    if (k < 0)
    {
        printf("Error: Vehicle not found in the records.\n");
        return;
    }
    int hours_spent = difftime(departure_time,arr[k].arrival_time)/3600;
    printf("Hours spent: %d hours\n", hours_spent);
    
    // Update the total parking hours for the vehicle
    arr[k].total_parking_hours += hours_spent;

    // Mark the parking space as free
    brr[space].status = 0;

    // Update membership if applicable
    if (arr[k].total_parking_hours > 200)
    {
        if (strcmp(arr[k].membership, "GOLDEN") != 0) // Upgrade to GOLDEN if not already GOLDEN
        {
            strcpy(arr[k].membership, "GOLDEN");
            printf("Membership upgraded to GOLDEN\n");
        }
    }
    else if (arr[k].total_parking_hours > 100)
    {
        if (strcmp(arr[k].membership, "PREMIUM") != 0) // Upgrade to PREMIUM if not already PREMIUM 
        {
            strcpy(arr[k].membership, "PREMIUM");
            printf("Membership upgraded to PREMIUM\n");
        }
    }

    // Calculate parking charges
    double charges = 0.0;
    if (strcmp(arr[k].membership, "NULL") == 0)
    {
        if (hours_spent <= 3)
        {
            charges = 100.0;
        }
        else
        {
            charges = (hours_spent - 3) * 50.0 + 100.0;
        }
        printf("Parking charges: %.2lf Rs\n", charges);
    }
    else
    { // Discount for users with membership
        if (hours_spent <= 3)
        {
            charges = 90.0;
        }
        else
        {
            charges = 0.9 * ((hours_spent - 3) * 50.0 + 100.0);
        }
        printf("Parking charges: %.2lf Rs\n", charges);
    }
    // Update the revenue generated by the parking space
    brr[space].revenue_generated += charges;
    // Update the total amount paid by the vehicle
    arr[k].total_amount_paid += charges;

    printf("Vehicle exited successfully!\nParking space ID %d is now free.\n", space + 1);
}
void swap_vehicle_details(struct vehicle_details arr[],int j)
{
	int temp;
	char tempe[NUM_LEN];
	temp=arr[j+1].parking_count;
    arr[j+1].parking_count=arr[j].parking_count;
    arr[j].parking_count=temp;
                           
    temp=arr[j+1].total_amount_paid;
    arr[j+1].total_amount_paid=arr[j].total_amount_paid;
    arr[j].total_amount_paid=temp;
                           
    temp=arr[j+1].total_parking_hours;
    arr[j+1].total_parking_hours=arr[j].total_parking_hours;
    arr[j].total_parking_hours=temp;
                           
    strcpy(tempe, arr[j+1].vehicle_number);
    strcpy(arr[j+1].vehicle_number, arr[j].vehicle_number);
    strcpy(arr[j].vehicle_number, tempe);
                           
    strcpy(tempe, arr[j+1].owner_name);
    strcpy(arr[j+1].owner_name, arr[j].owner_name);
    strcpy(arr[j].owner_name, tempe);
                                                      
    strcpy(tempe, arr[j+1].membership);
    strcpy(arr[j+1].membership, arr[j].membership);
    strcpy(arr[j].membership, tempe);
                                                      
}
void sort_by_parkingCount(struct vehicle_details arr[],int entry_count)
{
           int temp,i,j,k;
           char tempe[NUM_LEN];
           for(i=0;i<entry_count;i++)
           {
                for(j=0;j<entry_count-1;j++)
                {
                     if(arr[j].parking_count<arr[j+1].parking_count)
                     {
                       swap_vehicle_details(arr,j); 
                     }
                }
           }
           printf("Report on most parked vehicles :\n");
           for(k=0;k<entry_count;k++)
           {
                printf("Number: %d\n ",k+1);
                printf("Vehicle number: ");
                puts(arr[k].vehicle_number);
                printf("\n Owner name: ");
                puts(arr[k].owner_name);
                printf("\n Parking count :%d  ",arr[k].parking_count);
                printf("\n");     
           }
}               
void sort_by_parking_amount_paid(struct vehicle_details arr[],int entry_count)
{
           int temp,i,j,k;
           char tempe[NUM_LEN];
           for(i=0;i<entry_count;i++)
           {
                for(j=0;j<entry_count-1;j++)
                {
                     if(arr[j].total_amount_paid<arr[j+1].total_amount_paid)
                     {
                       swap_vehicle_details(arr,j);     
                     }
                }
           }
           printf("Report on total amount paid by vehicles :\n");
           for(k=0;k<entry_count;k++)
           {
                printf("Number  %d: ",k+1);
                printf("\nVehicle number: ");
                puts(arr[k].vehicle_number);
                printf("\nOwner name: ");
                puts(arr[k].owner_name);
                printf("\nTotal amount paid : %lf Rs \n ",arr[k].total_amount_paid);
               
           }
}
void swap_parking_details(struct parking_space_status brr[],int j)
{
    int temp;
	temp=brr[j+1].occupancy_count;
    brr[j+1].occupancy_count=brr[j].occupancy_count;
    brr[j].occupancy_count=temp;
                           
    temp=brr[j+1].parking_space_ID;
    brr[j+1].parking_space_ID=brr[j].parking_space_ID;
    brr[j].parking_space_ID=temp;
                           
    temp=brr[j+1].revenue_generated;
    brr[j+1].revenue_generated=brr[j].revenue_generated;
    brr[j].revenue_generated=temp;
        	
}
void sort_by_occupancy_count(struct parking_space_status arr[])
{
     int i,j,k;
	 struct parking_space_status brr[SPACE_DB_LEN];
     for(i=0;i<SPACE_DB_LEN;i++)
     {
     	brr[i].occupancy_count=arr[i].occupancy_count;
     	brr[i].parking_space_ID=arr[i].parking_space_ID;
     	brr[i].revenue_generated=arr[i].revenue_generated;
	 }
     
           int temp;
           for(i=0;i<SPACE_DB_LEN;i++)
           {
                for(j=0;j<SPACE_DB_LEN-1;j++)
                {
                     if(brr[j].occupancy_count<brr[j+1].occupancy_count)
                     {
                        swap_parking_details(brr,j);  
                     }
                }
           }
           printf("Report on most occupied parking spaces :\n");
           for(k=0;k<SPACE_DB_LEN;k++)
           {
                printf("Number  %d: ",k+1);
                printf("\nSpace ID: %d ",brr[k].parking_space_ID);
                printf("\nOccupancy count : %d ",brr[k].occupancy_count);
                printf("\n");
                
          }
}
void sort_by_revenue_generated(struct parking_space_status arr[])
{
     int i,j,k;
	 struct parking_space_status brr[SPACE_DB_LEN];
     for(i=0;i<SPACE_DB_LEN;i++)
     {
     	brr[i].occupancy_count=arr[i].occupancy_count;
     	brr[i].parking_space_ID=arr[i].parking_space_ID;
     	brr[i].revenue_generated=arr[i].revenue_generated;	
	 }
           double temp;
           for(i=0;i<SPACE_DB_LEN;i++)
           {
                for(j=0;j<SPACE_DB_LEN-1;j++)
                {
                     if(brr[j].revenue_generated < brr[j+1].revenue_generated)
                     {
                       swap_parking_details(brr,j);                               
                     }
                }
           }
           printf("Report on revenue generated by the parking spaces: \n");
           for(k=0;k<SPACE_DB_LEN;k++)
           {
                printf("Number  %d: ",k+1);
                printf("\nSpace ID: %d ",brr[k].parking_space_ID);
                printf("\nRevenue generated : %lf ",brr[k].revenue_generated);
                printf("\n");
                
          }
}
time_t parse_datetime(const char *datetime)
{
    struct tm tm;
    // Initialize the tm structure
    memset(&tm, 0, sizeof(struct tm));
    
    // Use sscanf to parse the datetime string
    if (sscanf(datetime, "%d-%d-%d %d:%d",
               &tm.tm_year,
               &tm.tm_mon,
               &tm.tm_mday,
               &tm.tm_hour,
               &tm.tm_min) != 5)
               {
        // Return -1 if parsing fails
        return -1;
    }
}

int main()
{
      //declaring an array of structure vehicle_details
      struct vehicle_details vehicle_DB[VEHICLE_DB_LEN];
      struct parking_space_status parking_status[SPACE_DB_LEN];
      initialize_vehicle_DB(vehicle_DB);
      initialize_parking_spaces(parking_status);
      struct vehicle_details vehicle;
      struct parking_space_status parkingSpace;
      char line[256];

       FILE *file1 = fopen("vehicle_details.txt", "r");
       
    if (file1 == NULL)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Read each line from the file
    int vehicle_count=0,i;
    while (fgets(line, sizeof(line), file1) && vehicle_count < 20)
    {
        char arrival[20], departure[20]; // Buffers for datetime strings
        // Use sscanf to parse the line
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%ld,%d,%d,%lf",
               vehicle_DB[vehicle_count].vehicle_number,
               vehicle_DB[vehicle_count].owner_name,
               arrival,
               departure,
              vehicle_DB[vehicle_count].membership,
               &vehicle_DB[vehicle_count].total_parking_hours,
               &vehicle_DB[vehicle_count].parking_space_ID,
               &vehicle_DB[vehicle_count].parking_count,
               &vehicle_DB[vehicle_count].total_amount_paid);

        // Parse arrival and departure times
              vehicle_DB[vehicle_count].arrival_time = parse_datetime(arrival);
              vehicle_DB[vehicle_count].departure_time = parse_datetime(departure);
        
              vehicle_count++;
    }

    fclose(file1);
    
    FILE *file2 = fopen("parking_space.txt", "r");
    if (file2 == NULL)
    {
        perror("Error opening file");
        return EXIT_FAILURE; // Exit if the file cannot be opened
    }
   
    // Read data from the file into the array of structures
    for (i = 0; i < SPACE_DB_LEN; i++)
    {
        if (fscanf(file2, "%d,%d,%d,%lf",
                   &parking_status[i].parking_space_ID,
                   &parking_status[i].status,
                   &parking_status[i].occupancy_count,
                   &parking_status[i].revenue_generated) != 4)
                   {
            fprintf(stderr, "Error reading data for parking space %d\n", i + 1);
            break; // Break if there is an issue reading the data
        }
    }

    // Close the file after reading
    fclose(file2);
    
      int registered_vehicles=20;
      printf("\n Welcome to the Smart Car Parking Lot System!\n");
      char vehicle_num[NUM_LEN];
                      char name[NAME_LEN];
                      int ID;

        int choice,subchoice;
        do
        {
         
             printf("1. vehicle Entry\n");
             printf("2. Vehicle Exit\n");
             printf("3. Sorting and analysis\n");
             printf("4. Exit system\n");
             printf("Please Enter your choice: \n");
             scanf("%d", &choice);
             switch(choice)
            {
                     case 1:  
                     // vehicle entry
                      printf("Enter your vehicle details\n");
                      printf("Enter the vehicle number:\n");
                      scanf(" %[^\n]",vehicle_num);
                      printf("Enter the name of owner:\n");
                      scanf(" %[^\n]",name);
                      
                     Update_Insert_vehicle_details(vehicle_DB,parking_status,vehicle_num,name,time(NULL),&registered_vehicles);
                     break;
                     
                      case 2:
                      // Vehicle exit  
                      printf("Enter your vehicle number:\n");
                      scanf(" %[^\n]", vehicle_num); // Prompt for the vehicle number during exit
                      printf("Enter your parking space ID:\n");
                      scanf("%d", &ID);
                      
                      exit_updation(vehicle_DB, parking_status, ID, vehicle_num, time(NULL), registered_vehicles);
                      break;
                      
                       case 3:
                      //sorting and analysis
                       do
                      {
                         printf("case 1: Sort the list of vehicles based on the number of parkings done\n");
                         printf("case 2: Sort the list of vehicles based on the total parking amount paid\n");
                         printf("case 3: Sort the parking spaces based on how often they are occupied\n");
                         printf("case 4: Sort the parking spaces based on the revenue generated \n");
                         printf("case 5:Exit sorting and analysis \n");
                         
                         printf("Enter your choice:\n");
                         scanf("%d",&subchoice);
                         switch(subchoice)
                         {
                              case 1:
                                   sort_by_parkingCount(vehicle_DB,registered_vehicles);
                                   break;
                              case 2:
                                  sort_by_parking_amount_paid(vehicle_DB,registered_vehicles);
                                   break;
                              case 3:
                                   sort_by_occupancy_count(parking_status);
                                   break;
                              case 4:
                                   sort_by_revenue_generated(parking_status);
                                   break;
                              case 5 :
                                   printf("Exiting analysis and sorting\n");
                                   break;
                              default:
                                   printf("Invalid choice! please enter a valid choice\n");
                                   break;
                          }
                      }
                      while(subchoice!=5);
                      
                      case 4:
                           printf("Exiting the system.Thank you!\n");
                           break;
                           
                      default:
                           printf("Invalid choice. Please enter a valid choice \n");
                           break;                          
                }
          }
          while(choice!=4);
return 0;
}

