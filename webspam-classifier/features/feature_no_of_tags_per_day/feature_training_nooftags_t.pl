my %table;
$count=1;
$sum = 0;
$avg = 0;
$daycount = 0;
open(TAS,"tas") or die "cant open TAS: $!";
open(TS,">NONSPAM") or die "cant open TS: $!";

while(<TAS>){
	chomp;
	my ($user_id , $tag, $content_id, $content_value, $date) = split /\s+/;
	#push @{$table{$user_id}} , $tag, $content_id, $content_value, $date; 
	push @{$table{$user_id}} , $date;

}

foreach $user_id (sort { $a <=> $b }keys %table){
	
		
		if(	$user_id % 3 == 0 && $user_id != 0 ) { 
		
		@values = @{$table{$user_id}};
		@comp = sort @values;
		$len = @comp;
		$i = 1;
		$sum = 0;
		$avg = 0;
		$daycount = 0;
		print TS "$user_id "; 
		print "$user_id   "; 		

			foreach $_ (sort @values) {
		
				if($comp[$i] eq $_) {$count++;$i++;print TSPAM "($_)  ";
					print  "($_)  ";next; }
		#			print TS "$comp[$i]  ";
		#			print  "$comp[$i]  ";
				else{
				print TSPAM "($_)  ";
					print  "($_)  ";
					$daycount++;
					$sum = $sum + $count;
					$count=1;
					$i++;
				}
				if ($i == $len+1) {
				print TSPAM "($_)  ";
					print  "($_)  ";
					$avg = $sum/$daycount;
					
					print TS "(4:$avg) ";
					print "($avg) "; 
					
				}
			}
		print TS "\n\n\n\n"; 
		print "\n\n\n\n"; 
		}
		

}

# print "\n\n$count";
close(TS);
close(TAS);


