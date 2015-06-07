import fileinput
import math
import random
import sys
import time

__author__ = 'ivan'


class ValuePtr:
    def __init__(self, value):
        self.value = value

    def get(self): return self.value

    def set(self, value): self.value = value

    def __repr__(self):
        return "<" + str(round(self.value,2)) + ">"


class Neuron:
    def __init__(self, weight_list, input_values, output):
        self.weight_list = weight_list
        self.input_values = input_values
        self.output = output

    def calculate_output(self):
        return self.activation_function(sum([x.get() * y.get() for x, y in zip(self.weight_list, self.input_values)]))

    def trigger(self):
        self.output.set(self.calculate_output())

    def activation_function(self, net):
        pass


class SigmoidalNeuron(Neuron):
    def __init__(self, weight_list, input_values, output):
        Neuron.__init__(self, weight_list, input_values, output)

    def activation_function(self, net):
        return 1.0 / (1 + math.exp(-net))


class IdentityNeuron(Neuron):
    def __init__(self, weight_list, input_values, output):
        Neuron.__init__(self, weight_list, input_values, output)

    def activation_function(self, net):
        return net


if len(sys.argv) != 2:
    print "usage: %s hidden_layer_n" % sys.argv[0]

hidden_layer_n = int(sys.argv[1])

print "Neural network training"
print " - architecture: 1 x %d x 1" % hidden_layer_n


class SimpleNeuralNetwork:
    def __init__(self, hidden_layer_n):
        self.hidden_layer_n = hidden_layer_n
        self.weights = [ValuePtr(1) for _ in xrange((1 + 1) * hidden_layer_n + (hidden_layer_n + 1) * 1)]
        self.values = [ValuePtr(1) for _ in xrange(1 + 1 + hidden_layer_n + 1)]

        fixed_value = self.values[0]
        self.ann_input_value = self.values[1]
        self.ann_hidden_neurons_outputs = self.values[2:-1]
        self.ann_output_value = self.values[-1]
        self.hidden_layer_neurons = [SigmoidalNeuron(
            self.weights[2 * x:2 * (x + 1)], [fixed_value] + [self.ann_input_value],
            self.ann_hidden_neurons_outputs[x]) for x in xrange(0, hidden_layer_n)]

        self.output_neuron = IdentityNeuron(self.weights[(1 + 1) * hidden_layer_n:],
                                            [fixed_value] + self.ann_hidden_neurons_outputs, self.ann_output_value)

    def set_input(self, value):
        self.ann_input_value.set(value)

    def run(self):
        for neuron in self.hidden_layer_neurons:
            neuron.trigger()
        self.output_neuron.trigger()

    def get_output(self):
        return self.ann_output_value

    def evaluate(self, value):
        self.set_input(value)
        self.run()
        return self.get_output().get()

    def get_weights(self):
        return self.weights

    def set_weights(self, new_weights):
        assert len(new_weights) == len(self.weights)

        for old, new in zip(self.weights, new_weights):
            assert isinstance(new, float)
            old.set(new)

    def get_weights_len(self):
        return len(self.weights)


class NewtorkTrainer:
    """

    :type ann: SimpleNeuralNetwork
    :type population: list(list(float))
    """

    def __init__(self, ann, population_cnt, mutation_sigma,iterations):
        """

        :param ann: SimpleNeuralNetwork
        :param population_cnt: int
        :param mutation_sigma: float
        :param desired_error: float
        :param iterations: int
        :return:
        """
        self.iterations = iterations
        random.seed(time.time())
        self.ann = ann
        self.population_cnt = population_cnt
        self.mutation_chance = ann.hidden_layer_n / float(ann.get_weights_len())
        self.mutation_sigma = mutation_sigma
        self.chromosome_cnt = self.ann.get_weights_len()

    def generate_initial_candidate(self):
        return [random.gauss(0, 10) for _ in xrange(0, self.chromosome_cnt)]

    def crossing_over(self, candidate_a, candidate_b):
        position = random.randint(0, self.chromosome_cnt - 1)
        crossed_part = [x/2+y/2 for x,y in zip( candidate_b[position:], candidate_a[position:])]

        return candidate_a[:position] +crossed_part, candidate_a[:position]+ crossed_part

    def mutatate(self, candidate):
        #print "-mutating:"
        #print candidate
        mutated =[x + (random.gauss(0, self.mutation_sigma) if random.random() < self.mutation_chance else 0) for x in
                candidate]
        #print mutated
        return mutated

    def calculate_fitness(self, candidate):
        """

        :param candidate: list(float)
        :return: float
        """
        self.ann.set_weights(candidate)
        return sum([pow(self.ann.evaluate(input) - output, 2) for input, output in self.train_samples])/len(self.train_samples)

    def evaluate_population(self):
        return [self.calculate_fitness(x) for x in self.population]

    def generate_new_population(self, fitnesses):
        #print "generating new population"

        population = [self.get_best_candidate(fitnesses)[1]]

        while len(population) < self.population_cnt:
            population.extend(self.generate_new_candidates(fitnesses))

        self.population = population[:self.population_cnt]

    def get_best_candidate(self, fitnesses):
        zipped = zip(fitnesses, self.population)
        zipped.sort(key=lambda x: x[0])
        return zipped[0]

    def generate_new_candidates(self, fitnesses):
        fitnesses_sum = sum(fitnesses)
        fitness_ratios = [x / fitnesses_sum for x in fitnesses]
        mother,idx1 = self.choose_candidate(fitness_ratios)
        father,idx2 = self.choose_candidate(fitness_ratios)
        while idx2 == idx1:
            father,idx2 = self.choose_candidate(fitness_ratios)
            #father = self.population[idx2+1] if idx2+1<len(self.population) else self.population[0]

        #print "-crossing over:"
        #print mother
        #print father
        #print "-"*20
        child1, child2 = self.crossing_over(mother, father)
        #print child1
        #print child2
        #print "="*20
        return [self.mutatate(child1), self.mutatate(child2)]

    def choose_candidate(self, fitness_ratios):
        choice = random.random()

        i = 0
        idx = 0
        for fitness, candidate in zip(fitness_ratios, self.population):
            i += fitness
            if choice < i:
                return candidate,idx
            idx+=1

        raise Exception("Unexpected")

    def train(self, train_samples, desired_error):
        self.desired_error = desired_error
        self.train_samples = train_samples
        self.population = [self.generate_initial_candidate() for _ in xrange(0, self.population_cnt)]

        it = 0
        while it<self.iterations:
            #print "population:"
            #for candidate in self.population:
            #    print candidate
            #print "evaluating:"
            it+=1
            fitnesses = self.evaluate_population()
            fitness, candidate = self.get_best_candidate(fitnesses)
            #print "fitnesses: "+str(fitnesses)
            if it %10 == 0:
                print "%7d"%it ,"|","MSE: "+str(fitness)
            if fitness <= self.desired_error:
                print fitness
                self.ann.set_weights(candidate)
                print "Num of iterations: %d" %it
                return self.ann

            self.generate_new_population(fitnesses)


neural_network = SimpleNeuralNetwork(hidden_layer_n)

trainer = NewtorkTrainer(neural_network,population_cnt= 5, mutation_sigma=0.5,iterations=10000)

train_set = []
for line in fileinput.input("training-set.txt"):
    x = [float(x.strip()) for x in line.split('\t')]
    train_set.append((x[0],x[1]))
print train_set
ann = trainer.train(train_set, 0.01)

print "end"
if ann is None:
    exit(0)
print ann.get_weights()

test_set = []
for line in fileinput.input("test-set.txt"):
    x = [float(x.strip()) for x in line.split('\t')]
    test_set.append((x[0],x[1]))

mse = 0
for input,output in test_set:
    ann_output = ann.evaluate(input)

    se = pow(ann_output-output,2)
    print u'Ulaz: {0:7.4f} Izlaz: {1:7.4f} Ocekivani izlaz: {2:7.4f} SE: {3:7.4f}'.format(input,ann_output,output,se)
    mse+=se

while True:
    input = float(raw_input("Unesi ulaz: "))
    ann_output = ann.evaluate(input)
    print u'Ulaz: {0:7.4f} Izlaz: {1:7.4f}'.format(input,ann_output)


mse/= len(test_set)
print "MSE:",mse