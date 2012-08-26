my %table;
my %table1;
$count = 0;
$usercount = 0;
$tagcount=0;
$loopcount = 0;
$valueslen = 0;
open(TAS_SPAM,"tas_spam") or die "cant open TAS_SPAM: $!";
open(TSPAM2,">tas_spam_tag_len") or die "cant open TSPAM2: $!";

while(<TAS_SPAM>){
	chomp;
	my ($user_id , $tag, $content_id, $content_value, $date) = split /\t+/;
	push @{$table{$user_id}} , $tag; 
	#$table{$content_id} = $user_id;

}

foreach $user_id (sort { $a <=> $b } keys %table){
	
		my $loopcount = 0;
		if(	$user_id % 32 == 0 ) { 
		
		
		@values = @{$table{$user_id}};
		$valueslen = @values;
		#print "$valueslen:";
		$count = 0;
		$lencount = 0;
		$loopcount = 0;
		print "$user_id ";	
		foreach $_ (@values) {
			
			$loopcount++;
			if(length($_) <= 2) {
			$count++;
			$lencount++;
		#	print "$lencount:$loopcount:";
			print " ($_ ) ";
			print TSPAM2 "$user_id\n";
				
			}
				if ($loopcount == $valueslen ){
				if( $lencount == 0 ) {	print TSPAM2 "$user_id\n";}
				if( $lencount == 1 ) {	print TSPAM2 "$user_id\n";}
				}	

	#	print "$user_id : $count "
		}		
			print " ($count) ";
			print "\n\n\n\n";
	#print TSPAM "$user_id : $count "; 
	 
	#		print TSPAM "\n\n\n\n"; 
	#		print "\n\n\n\n";	
		
}
		

}

print "count: $count\n";

close(TSPAM2);
open(TSPAM2,"tas_spam_tag_len") or die "cant open TSPAM2: $!";

open(TSPAM1,">train_tag_len") or die "cant open TSPAM1: $!";

while(<TSPAM2>){
	chomp;
	my ($user_id1) = split /\s+/;
	push @{$table1{$user_id1}} , 0; 
	#$table{$content_id} = $user_id;

}

foreach $user_id1 (sort { $a <=> $b } keys %table1){

	$tagcount = @{$table1{$user_id1}};
	if ($tagcount == 1) {$tagcount = 0;}
	$usercount++;
	#print TSPAM1 "+1 ";
	print TSPAM1 "$user_id1 ";
	print TSPAM1 "3:$tagcount";
	print TSPAM1 "\n";
	print "$user_id1: $tagcount";
	print "\n";

}
#print "$usercount\n";
close(TSPAM1);
close(TSPAM2);
close(TAS_SPAM);
