my %table;
my %temptable;


open(TASSPAM,"tas_spam") or die "cant open TASSPAM: $!";
open(TS,">SPAM") or die "cant open TS: $!";

while(<TASSPAM>){
	chomp;
	my ($user_id , $tag, $content_id, $content_value, $date) = split /\s+/;
	push @{$table{$user_id}} , $tag;

}

foreach $user_id (sort { $a <=> $b} keys %table){
	
		$sum = 0;
		$avg = 0;	
		$length = 0;
		if(	$user_id % 32 == 0 ) { 
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
				my ($u_id , $tag ) = split /\s+/;
				push @{$temptable{$tag}} , $u_id;
				}

				close(VAR);
				$length = keys(%temptable);	
					
					foreach $tag (keys %temptable) {
					$len = @{$temptable{$tag}};
					$sum += $len;
					print TS " ($tag   $len)";
					print " ($tag  $len)";
					
					}
					$avg = $sum / $length;
					undef %temptable;
					
					#print TS "$sum $length $avg"; 
					#print "$sum $length $avg"; 
					
					print TS " (5:$avg)"; 
					print " (5:$avg)"; 
					
					
					print TS "\n\n\n\n"; 
					print "\n\n\n\n"; 
		}
		

}

close(TS);
close(TASSPAM);

