my %table1;
my %table;
$loopcount = 0;


open(SPAM,"train6") or die "cant open SPAM: $!";
open(NONSPAM,"train6_nonspam") or die "cant open NONSPAM: $!";
open(TRAIN, ">svm_train_final") or die "cant open TRAIN: $!";
open(TEST, ">svm_test_final") or die "cant open TEST: $!";


while(<SPAM>){
	chomp;
	my ($user_id , $label, $feature1,$feature2,$feature3,$feature4,$feature5,$feature6,$feature7) = split /\s+/;
	push @{$table{$user_id}} ,  $label, $feature1,$feature2,$feature3,$feature4,$feature5,$feature6,$feature7; 
	
}

while(<NONSPAM>){
	chomp;
	my ($user_id1 , $label1, $feature11,$feature21,$feature31,$feature41,$feature51,$feature61,$feature71) = split /\s+/;
	push @{$table1{$user_id1}} ,  $label1, $feature11,$feature21,$feature31,$feature41,$feature51,$feature61,$feature71; 
	
}


foreach $user_id (sort { $a <=> $b } keys %table){
	
		$loopcount++;
		if(	$loopcount % 4 == 0 ) { 
		my @values = @{$table{$user_id}};
		print TEST join ' ', @values;
		print TEST "\n";
		}
		else {
		my @values = @{$table{$user_id}};
		print TRAIN join ' ', @values;
		print TRAIN "\n";
		
		}
		

}

$loopcount = 0;
foreach $user_id1 (sort { $a <=> $b } keys %table1){
	
		$loopcount++;
		if(	$loopcount % 4 == 0 ) { 
		my @values = @{$table1{$user_id1}};
		print TEST join ' ', @values;
		print TEST "\n";
		}
		else {
		my @values = @{$table1{$user_id1}};
		print TRAIN join ' ', @values;
		print TRAIN "\n";
		}
		
}



close(SPAM);
close(NONSPAM);
close(TEST);
close(TRAIN);