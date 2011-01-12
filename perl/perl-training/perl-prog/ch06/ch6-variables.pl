### Ch6. Perl variables

###-----------------------------
my @scalar;
my @array;
my %hash;


my $number = 3;


###-----------------------------
### Scalars
###-----------------------------
my $name = "Arthur";
my $whoami = 'Just Another Perl Hacker';
my $meaning_of_life = 42;
my $number_less_than_1 = 0.000001;
my $very_large_number = 3.27e17;

print "$name says,\n";


###-----------------------------
### Double quote, single quote
###-----------------------------
### p27

my $price = '$9.95';

my $invoice_item = "24 widgets at $price each\n";
print $invoice_item;


###-----------------------------
### Special characters
###-----------------------------
### p28

print 'Here\'s an example';



###-----------------------------
### Advanced variable interpolation
###-----------------------------
### p29

my $what = "jumper";
print "I have 4 $whats";
print "I have 4 " . $what . "s";
print "I have 4 ${what}s";

###-----------------------------
### Arrays
###-----------------------------
### p30
my @fruits = ("apples", "oranges", "quavas", "passionfruit", "grapes");
my @magin_numbers = (23, 43, 69);
my @random_scalars = ("numble", 123.45, "willy the wombat", -300);

print $fruits[0];
print $random_scalars[2];
@fruits[0] = "pineapples";

@fruits[1,2,3];
@fruits[3,1,2];
@magic_numbers[0..2];
@magic_numbers[1..5];

print "My favourite fruits are @fruits\n";
print "Two types of fruit are @fruits[0,2]\n";
print "My favourite fruit is $fruits[3]\n";

$fruits[-1];
$fruits[-3];

my $last = $#fruits;
my $number_of_fruits = $#fruits + 1;

my @months = qw(January February March April May June July August September October November December);

###-----------------------------
### Hashes
###-----------------------------
### p34

#my %monthdays = ("January", 31, "February", 28, "March", 31);
my %monthdays = (
    "January" => 31, 
    "February" => 28, 
    "March" => 31
);


###-----------------------------
### Special variables
###-----------------------------
### p38

