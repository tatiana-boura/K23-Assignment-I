## PROJECT K23A Εργασία 1
	Χίου Ρίτα Άννα sdi1700192
	Χουσιανίτη Κατερίνα sdi1700194
	Μπούρα Τατιάνα sdi1700100

## Αρχεία και οργάνωση
Η οργάνωση των αρχείων έχει γίνει ως εξής: η κάθε χρησιμοποιούμενη δομή έχει τον δικό της φάκελο(hash_table,list και tuples), τα datasets βρίσκονται σε δικό τους φάκελο(datasets), ό,τι σχετίζεται με το reading και η main βρίσκονται στον φάκελο reading, τα unit tests στον unit_testing, ενώ παρέχεται και makefile και το παρόν README.md.

## Compile and run
Για να τρέξει κατευθείαν από το repository, χρησιμοποιώντας τα αρχεία στο φάκελο dataset,
> >make  
> >./prog ./datasets/camera_specs/2013 ./datasets/sigmod_large_labelled_dataset.csv  
						ή  
> >./prog ./datasets/camera_specs/2013 ./datasets/sigmod_medium_labelled_dataset.csv,  
όπου sigmod_large_labelled_dataset.csv είναο το datasetW και το sigmod_medium_labelled_dataset το datasetY.

## Λειτουργικότητα / Σχεδιαστικές επιλογές

**Generic List**
Χρησιμοποιήσαμε μια generic λίστα καθώς παρατηρήσαμε πως ήταν απαραίτητη σε παραπάνω από μία υλοποιήσεις, όπως την αποθήκευση των ζευγών <όνομα_ιδιότητας,generic_list(τιμή_ιδιότητας)> (τα οποία ονομάζουμε tuples), τη δημιουργία κλικών από spec_ids και την διαχείριση του collision στα hashtable.
H append είναι ουσιαστικά put_at_front εφόσον δεν μας ενδιαφέρει η σειρά, αφού τότε έχει πολυπλοκότητα Ο(1).

**Hash Table**
Αποφασίσαμε ότι η πολυπλοκότητα για την πρόσβαση στο εκάστοτε spec_id είναι ιδιαίτερα σημαντική λόγω του πλήθους ζευγών που ελέγχονται κατά την ανάγνωση των dataset W,Y. Επομένως επιλέξαμε να αποθηκεύσουμε τα specs (spec_id,tuples,cliques) σε ένα hash table που κάθε στοιχείο του table δείχνει σε μία λίστα από buckets, τα οποία buckets αποθηκεύουν εγγραφές. Έτσι, κάθε εγγραφή σε κάθε bucket αποτελείται από το spec_id, μια λίστα με ζεύγη <propertyName, λίστα από τιμές propertyValueList (περισσότερα του ενός στοιχεία σε περίπτωση πίνακα)> και μία λίστα από spec_ids που δηλώνει την κλίκα των spec που ταιριάζουν. 
Το μέγεθος του hash καθορίζεται από το συνολικό πλήθος των καμερών, καθώς στην αρχή του προγράμματος μετράμε τους φακέλους και το πόσες κάμερες έχουν μέσα, διατηρώντας τον συνολικό αριθμό.

**Διάβασμα από dataset X**
Το πρόγραμμα για να εισάγει τα δεδομένα από το dataset X στις δομές που έχουμε επιλέξει σχεδιαστικά ακουλουθεί την εξής διαδικασία:
Ανοίγει κάθε έναν από τους καταλόγους που αντιστοιχούν στις ιστοσελίδες διαδικτυακών καταστημάτων και επεξεργάζεται σειριακά όλα τα αρχεία .json που περιέχονται σε αυτούς.
Ολοκληρώνοντας την ανάγνωση του dataset X κάθε αρχείο .json έχει μετατραπεί σε μία λίστα ζευγών <όνομα_ιδιότητας,generic_list(τιμή_ιδιότητας)> (tuples) και έχει εισαχθεί στην δομή Hash Table με κλειδί της μορφής: 					<όνομα_ιστοτόπου>//<όνομα_αρχείου_χωρίς_την_κατάληξη_.json> 
έτσι ώστε να υποστηρίζει την μετέπειτα λειτουργία του προγράμματος.

Πιο συγκεκριμένα, για την μετατροπή κάθε αρχείου .json στη μορφή λίστας, το πρόγραμμα διαβάζει το αρχείο γραμμή-γραμμή, με χρήση της συνάρτησης fgets(char *str, int n, FILE *stream), και στη συνέχεια ελέγχοντας το περιεχόμενο του buffer(str) συνεχίζει σε μία από τις παρακάτω ενέργειες:

1)Αν περιέχει τους χαρακτήρες '{' η '}', οι οποίοι και οριοθετούν τα περιεχόμενα του αρχείου, το πρόγραμμα προχωράει στην ανάγνωση της επόμενης γραμμής του αρχείου.

2)Αν περιέχει τον χαρακτήρα '[' και μετά ακολουθεί αλλαγή γραμμής '\n', τότε έχει εντοπιστεί εγγραφή στο αρχείο με την μορφή πίνακα.
Σε αυτή την περίπτωση το πρόγραμμα συνεχίζει την ανάγνωση και αποθηκεύει κάθε γραμμή του αρχείου που αφορά το συγκεριμένο ζεύγος (όνομα_ιδιότητας,τιμή_ιδιότητας) σε βοηθητικό buffer χρησιμοποιώντας τον ειδικό χαρακτήρα '#' για τον διαχωρισμό των γραμμών. Το τέλος ενός πίνακα σηματοδοτείται από από τον εντοπίσμό γραμμής που περιέχει μόνο την αλληλουχία χαρακτήρων ']'και',' ή ']'και'\n'. Στην συνέχεια καλεί την συνάρτηση  json_array_handler(char* str, TuplePtr t) στην οποία το όρισμα str αποτελεί μία συμβολοσειρά της μορφής "όνομα_ιδιότητας#τιμή_ιδιότητας_1#τιμή_ιδιότητας_2#...#τιμή_ιδιότητας_n#". H συνάρτηση αυτή διαχωρίζει  κατάλληλα τη συμβολοσειρά str και αρχικοποιεί την δομή TuplePtr t περνώντας το όνομα_ιδιότητας και εισάγωντας διαδοχικά τις τιμές ιδιότητας στην generic λίστα της δομής.

3)Αν δεν ισχύει κάποια από τις περιπτώσεις 1) και 2) τότε το buffer περιέχει μία γραμμή στην οποία υπάρχει ένα ζεύγος (όνομα_ιδιότητας,τιμή_ιδιότητας).Τότε καλείται η συνάρτηση json_separator(char* buff,TuplePtr t) η ποία διαχωρίζει  κατάλληλα τη συμβολοσειρά str και αρχικοποιεί την δομή TuplePtr t περνώντας το όνομα_ιδιότητας και εισάγωντας την τιμή ιδιότητας στην generic λίστα της δομής. 

**Διαχείση ταιριασμάτων**
Όταν διαβάσουμε από το dataset W ένα ζεύγος left_spec_id, right_spec_id τα όποια είναι ίδια (έχουν δηλαδή label να ισούται με 1), hash-άρουμε το κάθε προαναφερθέν id, βρίσκουμε σε ποιά θέση του hash table βρίσκεται και ύστερα με τη συνάρτηση foundInHT επιστρέφουμε το bucket στο οποίο ανήκει το συγκεκριμένο id καθώς και τη θέση που καταλαμβάνει στο bucket(δηλαδή ποιά εγγραφή είναι). Αφότου έχουμε τις ακριβείς θέσεις και των 2 spec id, καλούμε τη συνάρτηση changePointers η οποία αφότου ελέγξει ότι δεν αναφερόμαστε στην ίδια κλίκα(δηλαδή αποφεύγει φαινόμενα κυκλισμού), κάνει merge τις δύο κλίκες (λίστες) και σε κάθε στοιχείο της merged πια κλίκας πηγαίνει και αλλάζει τον pointer του προκειμένου όλα τα στοιχεία της κλίκας να δείχνουν(άρα και να αναφέρονται) στην ίδια κλίκα. 

**Memory reusability**
Γενικά, το πρόγραμμα όπου είναι εφικτό δεν κάνει alloc αλλά χρησιμοποιεί ήδη δεσμευμένη μνήμη προκειμένου να μην γίνονται σπατάλες. Για παράδειγμα, τα spec_ids στην κλίκα είναι αποθηκευμένα στις ίδιες θέσεις μνήμης με το spec_id(εν ονόματι path) στην εγγραφή του bucket. Ομοίως, οι ίδιες κλίκες είναι αποθηκευμένες στην ίδια θέση μνήμης.

**Εκτύπωση αποτελέσματος**
Προκειμένου να δημιουργηθεί το αρχείο εξόδου output.txt, καλείται η συνάρτηση makeOutputFile της hash η οποία για κάθε εγγραφή των bucket στο hash table επισκέπτεται την κλίκα (εφόσον δεν την έχει ξαναεπισκεφτεί) και μέσω της συνάρτησης makeOutputFileList εκτυπώνει τις κλίκες με την εξής λογική προκειμένου να αποφύγει τα διπλότυπα: Έστω μια κλίκα με τα στοιχεία a b c. Στην αρχή, εκτυπώνει το a σε ζεύγος με τα δεξιά του, δηλαδή ab ac. Έπειτα, κάνει το ίδιο με το b. Το c δεν έχει δεξί, συνεπώς η εκτύπωση της κλίκας τερματίζει. Δεν εκτυπώνει τα αριστερά, διότι έτσι θα είχαμε διπλότυπα. 

**Unit Testing**
Εφαρμόζουμε unit testing σε όλες τις δομές που έχουμε χρησιμοποιήσει (generic list, hashTable, tuple structure) λαμβάνοντας υπόψιν τις πολλαπλές μορφές της λίστας στο πρόγραμμά μας. Οι μόνες συναρτήσεις που δεν ελέγχονται από το testing είναι αυτές που ασχολούνται με το input-output του προγράμματος, αυτές που αποδεσμεύουν μνήμη(αν και το πρόγραμμα δεν έχει κανένα leak ή error σύμφωνα με το Valgrind) και κάποιες μικρές συναρτήσεις εντός των δομών που ελέγχονται έμμεσα, αφού αποτελούν τμήμα μεγαλύτερων συναρτήσεων που ελέγχονται κανονικά, βλέπε η getBucket η οποία ελέγχεται έμεσα από τον έλεγχο της foundInHT. 
