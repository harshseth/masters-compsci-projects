my %table;
my %table1;


open(SPECHAR,"train3") or die "cant open SPECHAR: $!";
open(TRAIN1,">train4") or die "cant open TRAIN1: $!";

while(<SPECHAR>){
	chomp;
	my ($user_id , $label, $feature1,$feature2,$feature3,$feature4) = split /\s+/;
	push @{$table{$user_id}} , $label, $feature1, $feature2,$feature3,$feature4;

}

open(BAG,"avg_bptpu_spam") or die "cant open BAG: $!";

while(<BAG>){
	chomp;
	my ($user_id1 ,$feature5) = split /\s+/;
	$table1{$user_id1} = $feature5;

}


foreach $user_id (sort { $a <=> $b} keys %table){
	
		foreach $user_id1 (sort { $a <=> $b} keys %table1) {
				
				if( $user_id1 == $user_id ) 
				{ 
					push @{$table{$user_id}}, $table1{$user_id1}; 
					print TRAIN1 "$user_id ";
					
				    my @values = @{$table{$user_id}};
					
					print TRAIN1 join ' ', @values ;
					
					print TRAIN1 "\n";
					break; 
				} 
 		}
}

close(TRAIN1);
close(BAG);
close(SPECHAR);
