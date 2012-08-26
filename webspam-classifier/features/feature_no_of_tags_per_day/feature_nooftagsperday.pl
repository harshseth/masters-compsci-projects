my %table;
$count=1;
$sum = 0;
$avg = 0;
$daycount = 0;
open(TAS_SPAM,"tas_spam") or die "cant open TAS_SPAM: $!";
open(TSPAM,">SPAM") or die "cant open TSPAM: $!";

while(<TAS_SPAM>){
	chomp;
	my ($user_id , $tag, $content_id, $content_value, $date) = split /\s+/;
	#push @{$table{$user_id}} , $tag, $content_id, $content_value, $date; 
	push @{$table{$user_id}} , $date;

}

foreach $user_id (sort { $a <=> $b }keys %table){
	
		
		if(	$user_id % 32 == 0 ) { 
		
		@values = @{$table{$user_id}};
		@comp = sort { $a <=> $b } @values;
		$len = @comp;
		$i = 1;
		$sum = 0;
		$avg = 0;
		$daycount = 0;
		print TSPAM "$user_id "; 
		print "$user_id   "; 		

			foreach $_ (sort @values) {
		
				if($comp[$i] eq $_) {$count++;$i++; 
					print TSPAM "($_)  ";
					print  "($_)  ";
					next;
					}
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
					
					print TSPAM "(4:$avg )";
					print "($avg )"; 
					
				}
			}
		print TSPAM "\n\n\n\n"; 
		print "\n\n\n\n"; 
		}
		

}

# print "\n\n$count";
close(TSPAM);
close(TAS_SPAM);


