my %table;
my %temptable;


open(TASSPAM,"tas") or die "cant open TASSPAM: $!";
open(TS,">avg_tpbpu") or die "cant open TS: $!";

while(<TASSPAM>){
	chomp;
	my ($user_id , $tag, $content_id, $content_value, $date) = split /\s+/;
	push @{$table{$user_id}} , $content_id;

}

foreach $user_id (sort { $a <=> $b} keys %table){
	
		$sum = 0;
		$avg = 0;	
		$length = 0;
		if(	$user_id % 3 == 0 && $user_id != 0 ) { 
		my %temptable;	
		print TS "$user_id  ";
		print "$user_id  ";
		@values = @{$table{$user_id}};
		

			open(TSPA,">$user_id") or die "cant open TSPA: $!";
		
			foreach $_ (sort @values) {
			print TSPA "$user_id  $_";
			print TSPA "\n";
		
			}
		
			close(TSPA);
				
					
			
				open(VAR,"$user_id") or die "cant open VAR: $!";
		
				while(<VAR>){
				chomp;
				my ($u_id , $content_id ) = split /\s+/;
				push @{$temptable{$content_id}} , $u_id;
				}

				close(VAR);
				$length = keys(%temptable);	
					
					foreach $content_id (keys %temptable) {
					$len = @{$temptable{$content_id}};
					$sum += $len;
			#		print TS " $content_id   $len";
			#		print " $content_id  $len";
					
					}
					$avg = $sum / $length;
					undef %temptable;
					
				#	print TS "$sum $length $avg"; 
				#	print "$sum $length $avg"; 
					
					print TS " 8:$avg"; 
					print " 8:$avg"; 
					
					
					print TS "\n"; 
					print "\n"; 
		}
		

}

close(TS);
close(TASSPAM);

