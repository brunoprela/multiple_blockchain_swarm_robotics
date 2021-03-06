// external
import express from 'express';
var childProcess = require('child_process');
import { graphqlExpress, graphiqlExpress } from 'apollo-server-express';
import { makeExecutableSchema, addMockFunctionsToSchema } from 'graphql-tools';
import { fileLoader, mergeTypes, mergeResolvers } from 'merge-graphql-schemas';
import bodyParser from 'body-parser';
import path from 'path';
import cors from 'cors';
// app
import models from './models';

const GRAPHQL_PORT = 9000;
const GRAPHQL_ENDPOINT = '/graphql';
// import types from schema directory
const typeDefs = mergeTypes(fileLoader(path.join(__dirname, './schema')));
// import resolvers from resolvers directory
const resolvers = mergeResolvers(fileLoader(path.join(__dirname, './resolvers')));

export const schema = makeExecutableSchema({
    typeDefs,
    resolvers,
});

const app = express();

app.use(cors('*'));

app.use('/graphql', bodyParser.json(), graphqlExpress((req) => ({
        schema,
        context: {
            models,
            req
        }
    }))
);

app.use('/graphiql', graphiqlExpress({ endpointURL: GRAPHQL_ENDPOINT }));


// force: true drops your database every time you start the server
// logging: true allows you to see all the database logging
models.sequelize.sync({force: true, logging: false}).then(() => {
    app.listen(GRAPHQL_PORT, () => console.log(
        `GraphiQL is now running on http://localhost:${GRAPHQL_PORT}${GRAPHQL_ENDPOINT}`
    ));
});
