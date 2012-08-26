

my %table;
my %table1;
$count = 0;
$usercount = 0;
$tagcount=0;
$loopcount = 0;
$valueslen = 0;
open(TAS_SPAM,"tas_spam") or die "cant open TAS_SPAM: $!";
open(TSPAM2,">tas_spam_bag_words") or die "cant open TSPAM2: $!";

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
			if(($_ =~ m /hardcore/i)||($_ =~ m/adult/i)||($_ =~ m/investing/i)||($_ =~ m /pregnancy/i)||($_ =~ m/gay/i)||($_ =~ m/interracial/i)||($_ =~ m/xxx/i)||($_ =~ m/erotic/i)||($_ =~ m/penis/i)||($_ =~ m/bankruptcy/i)||($_ =~ m/divorce/i)||($_ =~ m/residual/i)||($_ =~ m/bond/i)||($_ =~ m/boobs/i)||($_ =~ m/tits/i)||($_ =~ m/massage/i)||($_ =~ m/breast/i)||($_ =~ m/Amateur/i)||($_ =~ m/lesbians/i)||($_ =~ m/handjob/i)||($_ =~ m/cocks/i)||($_ =~ m/casino/i)||($_ =~ m/relationship/i)||($_ =~ m/infection/i)||($_ =~ m/terrorist/i)||($_ =~ m/terrorism/i)||($_ =~ m/softcore/i)||($_ =~ m/horror/i)||($_ =~ m/wealth/i)||($_ =~ m/worm/i)||($_ =~ m/masturbate/i)||($_ =~ m/rental/i)||($_ =~ m/dating/i)||($_ =~ m/poker/i)||($_ =~ m/underwear/i)||($_ =~ m/bra/i)||($_ =~ m/acid/i)||($_ =~ m/toys/i)||($_ =~ m/credit/i)||($_ =~ m/debt/i)||($_ =~ m/smoking/i)||($_ =~ m/pistol/i)||($_ =~ m/secrets/i)||($_ =~ m/scams/i)||($_ =~ m/therapy/i)||($_ =~ m/fraud/i)||($_ =~ m/mortgages/i)||($_ =~ m/nudes/i)||($_ =~ m/auction/i)||($_ =~ m/chicks/i)) {
			$count++;
			$lencount++;
		#	print "$lencount:$loopcount:";
			print " ($_) ";
			print TSPAM2 "$user_id\n";
				
			}
				if ($loopcount == $valueslen ){
				if( $lencount == 0 ) {	print TSPAM2 "$user_id\n";}
				if( $lencount == 1 ) {	print TSPAM2 "$user_id\n";}
				}	

		
		}
		print " ( $count )";
			print "\n\n\n\n";
	#print TSPAM "$user_id : $count "; 
	 
	#		print TSPAM "\n\n\n\n"; 
	#		print "\n\n\n\n";	
		
}
		

}

print "count: $count\n";

close(TSPAM2);
open(TSPAM2,"tas_spam_bag_words") or die "cant open TSPAM2: $!";

open(TSPAM1,">train_bag_words") or die "cant open TSPAM1: $!";

while(<TSPAM2>){
	chomp;
	my ($user_id1) = split /\s+/;
	push @{$table1{$user_id1}} , 0; 
	#$table{$content_id} = $user_id;

}

foreach $user_id1 (sort { $a <=> $b }  keys %table1){

	$tagcount = @{$table1{$user_id1}};
	if ($tagcount == 1) {$tagcount = 0;}
	$usercount++;
	#print TSPAM1 "+1 ";
	print TSPAM1 "$user_id1 " ;
	print TSPAM1 "2:$tagcount";
	print TSPAM1 "\n";
	print "$user_id1: $tagcount";
	print "\n";

}
#print "$usercount\n";
close(TSPAM1);
close(TSPAM2);
close(TAS_SPAM);
