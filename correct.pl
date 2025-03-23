open(F,"main.asm") || die "can't open 'main.asm'\n";

open O0, ">main_cr.asm";
open O1, ">main_cr.inc";

$main_found = 0;

while (<F>) {

        if (!(/[\t+]PUBLIC/) && !(/[\t+]EXTRN/)) {

                next if (/^DGROUP/ || /lea       eax,\[eax\]/ || /nop/);

                if (/ASSUME/) {
                        print O0 "\n\t\tinclude start.inc\n";
                } else {
                        if (/^main_/) {
                                $main_found = 1;
                                print O1 $_;
                                next;
                        }
                
                        next if (($main_found == 1) && (/push/ || /enter/));

                        if ($main_found && (/leave/ || /add       esp,/)) {
                                $main_found = 2;
                                next;
                        }

                        if ($main_found == 2) {
                                next if (/pop/);
                                $main_found = 0;
                        }
                                
                        next if (/__CHP/);

                        if (/IF@/) { $_ = "    fxch      st(1)\n    fpatan\n"; }

                        if ($main_found == 0) { print O0 $_; } else
                                              { print O1 $_; }
                }
        }
}

close F;
close O0;
close O1;
